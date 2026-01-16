/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Server
*/
#include "Server.hpp"

#include "SFML/Network/TcpSocket.hpp"
#include "custom_packet/CustomPacket.hpp"
#include "packet_disassembler/PacketDisassembler.hpp"
#include "packet_factory/PacketFactory.hpp"
#include <iostream>
#include <thread>
#include <utility>

namespace server {
    Server::Server(const std::shared_ptr<ServerSharedData> &data):
   _sharedData(data) {}


    int Server::bindPorts(const uint16_t port)
    {
        _listener.setBlocking(false);
        _udpSocket.setBlocking(false);
        if (_listener.listen(port) != sf::Socket::Status::Done) {
            return 1;
        }
        if (_udpSocket.bind(port) != sf::Socket::Status::Done) {
            return 1;
        }
        return 0;
    }

    void Server::close()
    {
        _listener.close();
        _udpSocket.unbind();
    }

    [[noreturn]] void Server::_handleTcp()
    {
        while (true) {
            if (_socketSelector.wait()) {
                _checkSocket();
            }
        }
    }

    void Server::_checkSocket()
    {
        if (_socketSelector.isReady(_listener)) {
            _acceptConnection();
        } else {
            _handleNewTcpPacket();
        }
    }

    void Server::_handleNewTcpPacket()
    {
        for (auto it = _socketVector.begin(); it != _socketVector.end(); ) {
            auto &sock = **it;

            if (!_socketSelector.isReady(sock)) {
                ++it;
                continue;
            }

            cmn::CustomPacket packet;
            sf::Socket::Status const status = sock.receive(packet);

            if (status != sf::Socket::Status::Done) {
                if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
                    _socketSelector.remove(sock);
                    int const playerId = _getPlayerIdFromSocket(sock);
                    if (playerId != -1) {
                        _sharedData->deletePlayer(playerId);
                        _playerSocketMap.erase(playerId);
                    }
                    it = _socketVector.erase(it);
                    std::cout << "Player " << playerId << " removed" << std::endl;
                    continue;
                }
                ++it;
                continue;
            }
            int const playerId = _getPlayerIdFromSocket(sock);
            if (playerId != -1) {
                _routePacket(packet, playerId);
            }
            ++it;
        }
    }

    void Server::_routePacket(cmn::CustomPacket& packet, int playerId) const
    {
        auto data = cmn::PacketDisassembler::disassemble(packet);
        if (!data.has_value()) {
            return;
        }
        if (_isSystemPacket(data.value())) {
            _sharedData->addSystemPacket(data.value());
            return;
        }
        int const lobbyId = _sharedData->getPlayerLobby(playerId);
        if (lobbyId != -1) {
            _sharedData->addLobbyTcpReceivedPacket(lobbyId, data.value());
        }
    }

    bool Server::_isSystemPacket(const cmn::packetData& data)
    {
        return std::visit([](auto &&arg) -> bool {
            using T = std::decay_t<decltype(arg)>;
            return std::is_same_v<T, cmn::selectModeData> ||
                          std::is_same_v<T, cmn::requestJoinLobbyData> ||
                          std::is_same_v<T, cmn::leaveLobbyData>;
        }, data);
    }

    int Server::_getPlayerIdFromSocket(const sf::TcpSocket& socket) const
    {
        const auto port = socket.getRemotePort();
        const auto ip = socket.getRemoteAddress();

        if (!ip.has_value()) {
            return -1;
        }
        for (auto player : _sharedData->getMapPlayers()) {
            if (std::cmp_equal(player.second.first , port) &&
                player.second.second == ip.value()) {
                return player.first;
                }
        }
        return -1;
    }

    int Server::_getPlayerIdFromUdp(const sf::IpAddress &ip, unsigned short port) const
    {
        for (const auto& [playerId, playerInfo] : _sharedData->getMapPlayers()) {
            if (std::cmp_equal(playerInfo.first, port) && playerInfo.second == ip) {
                return playerId;
            }
        }
        return -1;
    }

    int Server::sendUdp(cmn::CustomPacket packet, const sf::IpAddress& clientIp, uint16_t port)
    {
        sf::Socket::Status const status = _udpSocket.send(packet, clientIp, port);
        if (status != sf::Socket::Status::Done) {
            return 1;
        }
        return 0;
    }

    int Server::sendTcp(cmn::CustomPacket packet, sf::TcpSocket& clientSocket)
    {
        sf::Socket::Status const status = clientSocket.send(packet);
        if (status != sf::Socket::Status::Done) {
            return 1;
        }
        return 0;
    }

    void Server::sendTcpToPlayer(int playerId, const cmn::CustomPacket& packet)
    {
        auto it = _playerSocketMap.find(playerId);
        if (it != _playerSocketMap.end() && it->second) {
            sendTcp(packet, *it->second);
        }
    }

    void Server::broadcastTcp(const cmn::CustomPacket& packet) const
    {
        for (const auto &client : _socketVector) {
            sendTcp(packet, *client);
        }
    }

    void Server::broadcastUdp(const cmn::CustomPacket& packet)
    {
        for (const auto &client : _socketVector) {
            auto ip = client->getRemoteAddress();
            const auto clientPort = client->getRemotePort();
            if (!ip.has_value() || clientPort == 0) {
                continue;
            }
            sendUdp(packet, ip.value(), clientPort);
        }
    }

    void Server::broadcastTcpToLobby(const int lobbyId, const cmn::CustomPacket& packet)
    {
        auto const playerIds = _sharedData->getLobbyPlayers(lobbyId);

        for (int const playerId : playerIds) {
            auto it = _playerSocketMap.find(playerId);
            if (it != _playerSocketMap.end() && it->second) {
                sendTcp(packet, *it->second);
            }
        }
    }

    void Server::broadcastUdpToLobby(const int lobbyId, const cmn::CustomPacket& packet)
    {
        auto const playerIds = _sharedData->getLobbyPlayers(lobbyId);

        for (int const playerId : playerIds) {
            auto playerInfo = _sharedData->getPlayer(playerId);
            if (playerInfo.has_value()) {
                sendUdp(packet, playerInfo->second, playerInfo->first);
            }
        }
    }

    void Server::_acceptConnection()
    {
        static int idPlayer = 1;
        auto client = std::make_shared<sf::TcpSocket>();

        client->setBlocking(false);

        if (_listener.accept(*client) != sf::Socket::Status::Done) {
            return;
        }
        _socketSelector.add(*client);

        _playerSocketMap[idPlayer] = client;
        _sharedData->addPlayer(idPlayer, client->getRemotePort(), client->getRemoteAddress().value(), client);
        sendTcp(cmn::PacketFactory::createConnectionPacket(idPlayer), *client);
        _socketVector.push_back(client);
        idPlayer++;
        std::cout << "New player " << idPlayer << " accepted" << std::endl;
    }

    void Server::_processLobbyPackets()
    {
        std::vector<int> const lobbyIds = _sharedData->getAllLobbyIds();

        for (int const lobbyId : lobbyIds) {
            auto tcpPacket = _sharedData->getLobbyTcpPacketToSend(lobbyId);
            if (tcpPacket.has_value()) {
                broadcastTcpToLobby(lobbyId, tcpPacket.value());
            }

            auto udpPacket = _sharedData->getLobbyUdpPacketToSend(lobbyId);
            if (udpPacket.has_value()) {
                broadcastUdpToLobby(lobbyId, udpPacket.value());
            }
        }

        if (auto tcpSinglePlayer = _sharedData->getTcpPacketToSendToSpecificPlayer()) {
            sendTcpToPlayer(tcpSinglePlayer->first, tcpSinglePlayer->second);
        }
    }

    void Server::run()
    {
        _socketSelector.add(_listener);
        _tcpThread = std::jthread{[this]{ _handleTcp(); }};

        std::optional<sf::IpAddress> sender;
        unsigned short port = 0;
        cmn::CustomPacket packet;

        while (true) {
            _processLobbyPackets();

            if (_udpSocket.receive(packet, sender, port) == sf::Socket::Status::Done) {
                if (!sender.has_value()) {
                    continue;
                }
                auto data = cmn::PacketDisassembler::disassemble(packet);
                if (data.has_value()) {
                    if (_isSystemPacket(data.value())) {
                        _sharedData->addSystemPacket(data.value());
                    } else {
                        int playerId = _getPlayerIdFromUdp(sender.value(), port);
                        if (playerId != -1) {
                            int lobbyId = _sharedData->getPlayerLobby(playerId);
                            if (lobbyId != -1) {
                                _sharedData->addLobbyUdpReceivedPacket(lobbyId, data.value());
                            }
                        }
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

}// namespace server
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
                    _sharedData->deletePlayer(sock.getRemotePort(), sock.getRemoteAddress().value());
                    it = _socketVector.erase(it);
                    continue;
                }
                ++it;
                continue;
            }

            // TODO: might not need player id if dont send tcp to game thread
            // except if we let ready as tcp
            int playerId = _getPlayerIdFromSocket(sock);
            if (playerId != -1) {
                _routePacket(packet, playerId);
                ++it;
            }
            ++it;
        }
    }

    void Server::_routePacket(cmn::CustomPacket& packet, int playerId) const
    {
        auto data = cmn::PacketDisassembler::disassemble(packet);
        if (!data.has_value())
            return;
        if (_isSystemPacket(data.value())) {
            _sharedData->addSystemPacket(packet);
            return;
        }
        int lobbyId = _sharedData->getPlayerLobby(playerId);
        if (lobbyId != -1) {
            _sharedData->addLobbyTcpReceivedPacket(lobbyId, packet);
        }
    }

    // TODO: create the packet no ? raphael is a retard btw
    // also isnt optimized, might add something in header ?
    bool Server::_isSystemPacket(const cmn::packetData& data) const
    {
        return std::visit([](auto &&arg) -> bool {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, packetSoloGame> ||
                          std::is_same_v<T, packetCreateLobby> ||
                          std::is_same_v<T, packetJoinLobby> ||
                          std::is_same_v<T, packetMatchMaking> ||
                          std::is_same_v<T, packetLeaveLobby>) {
                return true;
            }
            return false;
        }, data.content);
    }

    int Server::_getPlayerIdFromSocket(const sf::TcpSocket& socket) const
    {
        const auto port = socket.getRemotePort();
        const auto ip = socket.getRemoteAddress();

        if (!ip.has_value()) {
            return -1;
        }
        for (auto player : _sharedData->getMapPlayers()) {
            if (player.second.first == port &&
                player.second.second == ip.value()) {
                return player.first;
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

    void Server::broadcastTcp(const cmn::CustomPacket& packet) const
    {
        for (const auto &client : _socketVector) {
            if (sendTcp(packet, *client) == 1) {
            }
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
            if (sendUdp(packet, ip.value(), clientPort) == 1) {
            }
        }
    }

    void Server::broadcastTcpToLobby(const int lobbyId, const cmn::CustomPacket& packet) const
    {
        std::vector<int> playerIds = _sharedData->getLobbyPlayers(lobbyId);

        for (int playerId : playerIds) {
            auto playerInfo = _sharedData->getPlayer(playerId);
            if (!playerInfo.has_value()) {
                continue;
            }
            for (const auto &client : _socketVector) {
                auto ip = client->getRemoteAddress();
                auto port = client->getRemotePort();

                if (ip.has_value() &&
                    port == playerInfo->first &&
                    ip.value() == playerInfo->second) {
                    sendTcp(packet, *client);
                    break;
                    }
            }
        }
    }

    void Server::broadcastUdpToLobby(const int lobbyId, const cmn::CustomPacket& packet)
    {
        std::vector<int> playerIds = _sharedData->getLobbyPlayers(lobbyId);
        auto mapPlayerInfo = _sharedData->getMapPlayers();

        for (int playerId : playerIds) {
            auto playerInfo = _sharedData->getPlayer(playerId);
            if (playerInfo.has_value()) {
                sendUdp(packet, playerInfo->second, playerInfo->first);
            }
        }
    }

    void Server::_acceptConnection()
    {
        static int idPlayer = 1;
        auto client = std::make_unique<sf::TcpSocket>();

        client->setBlocking(false);

        if (_listener.accept(*client) != sf::Socket::Status::Done) {
            return;
        }
        _socketSelector.add(*client);
        _sharedData->addPlayer(idPlayer, client->getRemotePort(), client->getRemoteAddress().value());
        sendTcp(cmn::PacketFactory::createConnectionPacket(idPlayer), *client);
        _socketVector.push_back(std::move(client));
        idPlayer++;
    }

    // TODO might need to add a send to specific players
    void Server::_processLobbyPackets()
    {
        std::vector<int> lobbyIds = _sharedData->getAllLobbyIds();

        for (int lobbyId : lobbyIds) {
            while (auto udpPacket = _sharedData->getLobbyUdpPacketToSend(lobbyId)) {
                broadcastUdpToLobby(lobbyId, udpPacket.value());
            }
            while (auto tcpPacket = _sharedData->getLobbyTcpPacketToSend(lobbyId)) {
                broadcastTcpToLobby(lobbyId, tcpPacket.value());
            }
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
                        _sharedData->addSystemPacket(packet);
                    } else {
                        // TODO: bitpacking is coming...
                        int lobbyId = _sharedData->getPlayerLobby(playerId);
                        if (lobbyId != -1) {
                            _sharedData->addLobbyUdpReceivedPacket(lobbyId, packet);
                        }
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}// namespace server
/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Server
*/

#ifndef R_TYPE_SERVER_HPP
#define R_TYPE_SERVER_HPP

#include "SFML/Network/IpAddress.hpp"
#include "SFML/Network/SocketSelector.hpp"
#include "SFML/Network/TcpListener.hpp"
#include "SFML/Network/TcpSocket.hpp"
#include "SFML/Network/UdpSocket.hpp"
#include "custom_packet/CustomPacket.hpp"
#include "packet_data/PacketData.hpp"
#include "packet_header/PacketHeader.hpp"
#include "server_shared_data/ServerSharedData.hpp"

#include <memory>
#include <thread>
#include "reliable_packet/ReliablePacket.hpp"
#include <vector>

namespace server {
    class Server {
    public:
        explicit Server(const std::shared_ptr<ServerSharedData> &data);

        int bindPorts(uint16_t port);
        void close();
        [[noreturn]] void run();

        int sendUdp(cmn::CustomPacket packet, const sf::IpAddress &clientIp, uint16_t port);
        static int sendTcp(cmn::CustomPacket packet, sf::TcpSocket &clientSocket);
        void sendTcpToPlayer(int playerId, const cmn::CustomPacket &packet);
        void broadcastTcp(const cmn::CustomPacket &packet) const;
        void broadcastUdp(const cmn::CustomPacket &packet);
        void broadcastTcpToLobby(int lobbyId, const cmn::CustomPacket &packet);
        void broadcastUdpToLobby(int lobbyId, const cmn::CustomPacket &packet);

    private:
        [[noreturn]] void _handleTcp();
        void _checkSocket();
        void _handleNewTcpPacket();
        std::unordered_map<uint32_t, cmn::reliablePacket> _reliablePackets;
        void _handleUdpReception(cmn::packetHeader header, cmn::packetData data, int lobbyId);
        void _resendTimedOutPackets();
        void _acceptConnection();
        void _processLobbyPackets();
        void _routePacket(const cmn::packetData &packet, int playerId) const;

        int _getPlayerIdFromSocket(const sf::TcpSocket &socket) const;
        int _getPlayerIdFromUdp(const sf::IpAddress &ip, unsigned short port) const;
        static bool _isSystemPacket(const cmn::packetData &data);

        sf::TcpListener _listener;
        sf::UdpSocket _udpSocket;
        sf::SocketSelector _socketSelector;
        std::vector<std::shared_ptr<sf::TcpSocket>> _socketVector;
        std::unordered_map<int, std::shared_ptr<sf::TcpSocket>> _playerSocketMap;
        std::jthread _tcpThread;
        std::shared_ptr<ServerSharedData> _sharedData;
    };

}// namespace server

#endif// R_TYPE_SERVER_HPP

/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Server
*/
#ifndef R_TYPE_SERVER_HPP
#define R_TYPE_SERVER_HPP

#include "SFML/Network/SocketSelector.hpp"
#include "SFML/Network/UdpSocket.hpp"
#include "custom_packet/CustomPacket.hpp"
#include "server_shared_data/ServerSharedData.hpp"
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <thread>

namespace server {

    class Server
    {
      public:
        explicit Server(const std::shared_ptr<ServerSharedData> &data);
        [[nodiscard]]int bindPorts(uint16_t port);
        void close();
        [[noreturn]] void run();

        int sendUdp(cmn::CustomPacket packet, const sf::IpAddress& clientIp, uint16_t port);
        static int sendTcp(cmn::CustomPacket packet, sf::TcpSocket& clientSocket);
        void broadcastUdp(const cmn::CustomPacket& packet);
        void broadcastTcp(const cmn::CustomPacket& packet)const;
        void broadcastUdpToLobby(int lobbyId, const cmn::CustomPacket& packet);
        void broadcastTcpToLobby(int lobbyId, const cmn::CustomPacket& packet) const;

      private:
        sf::TcpListener _listener;
        std::vector<std::unique_ptr<sf::TcpSocket>> _socketVector;
        sf::SocketSelector _socketSelector;
        sf::UdpSocket _udpSocket;
        std::jthread _tcpThread;
        std::shared_ptr<ServerSharedData> _sharedData;

        void _acceptConnection();
        [[noreturn]] void _handleTcp();
        void _checkSocket();
        void _handleNewTcpPacket();

        void _routePacket(cmn::CustomPacket& packet, int playerId) const;
        bool _isSystemPacket(const cmn::packetData& data) const;
        int _getPlayerIdFromSocket(const sf::TcpSocket& socket) const;
        void _processLobbyPackets();

    };

}// namespace server

#endif// R_TYPE_SERVER_HPP

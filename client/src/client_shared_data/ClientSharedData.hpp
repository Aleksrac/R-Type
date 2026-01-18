/*
** EPITECH PROJECT, 2026
** R_Type
** File description:
** client_shared_data
*/

#ifndef R_TYPE_CLIENT_SHARED_DATA_HPP
#define R_TYPE_CLIENT_SHARED_DATA_HPP

#include "packet_data/PacketData.hpp"
#include <mutex>
#include <optional>
#include <queue>

namespace client {
    class ClientSharedData {
    public:
        void addUdpReceivedPacket(const cmn::packetData &packet);
        std::optional<cmn::packetData> getUdpReceivedPacket();

        void addUdpPacketToSend(const cmn::packetData &packet);
        std::optional<cmn::packetData> getUdpPacketToSend();

        void addTcpReceivedPacket(const cmn::packetData &packet);
        std::optional<cmn::packetData> getTcpReceivedPacket();

        void addTcpPacketToSend(const cmn::packetData &packet);
        std::optional<cmn::packetData> getTcpPacketToSend();

        void stopGame();
        bool isGameRunning();

    private:
        std::queue<cmn::packetData> _udpReceivedQueue;
        std::queue<cmn::packetData> _udpSendQueue;
        std::queue<cmn::packetData> _tcpReceivedQueue;
        std::queue<cmn::packetData> _tcpSendQueue;
        std::mutex _mutex;

        bool _isRunning = true;
    };
}

#endif// R_TYPE_CLIENT_SHARED_DATA_HPP

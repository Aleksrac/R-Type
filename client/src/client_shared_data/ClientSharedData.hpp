/*
** EPITECH PROJECT, 2026
** R_Type
** File description:
** client_shared_data
*/

#ifndef R_TYPE_CLIENT_SHARED_DATA_HPP
#define R_TYPE_CLIENT_SHARED_DATA_HPP

#include "custom_packet/CustomPacket.hpp"
#include "packet_data/PacketData.hpp"
#include <mutex>
#include <optional>
#include <queue>

namespace client {
    class ClientSharedData {
    public:
        void addUdpReceivedPacket(const cmn::packetData &data);
        std::optional<cmn::packetData> getUdpReceivedPacket();

        void addUdpPacketToSend(const cmn::CustomPacket &packet);
        std::optional<cmn::CustomPacket> getUdpPacketToSend();

        void addTcpReceivedPacket(const cmn::packetData &data);
        std::optional<cmn::packetData> getTcpReceivedPacket();

        void addTcpPacketToSend(const cmn::CustomPacket &packet);
        std::optional<cmn::CustomPacket> getTcpPacketToSend();

    private:
        std::queue<cmn::packetData> _udpReceivedQueue;
        std::queue<cmn::CustomPacket> _udpSendQueue;
        std::queue<cmn::packetData> _tcpReceivedQueue;
        std::queue<cmn::CustomPacket> _tcpSendQueue;
        std::mutex _mutex;
    };
}

#endif// R_TYPE_CLIENT_SHARED_DATA_HPP

/*
** EPITECH PROJECT, 2026
** R_Type
** File description:
** client_shared_data
*/

#include "ClientSharedData.hpp"

namespace client {

    void ClientSharedData::addUdpReceivedPacket(const cmn::packetData &data)
    {
        std::lock_guard const lock(_mutex);
        _udpReceivedQueue.push(data);
    }

    std::optional<cmn::packetData> ClientSharedData::getUdpReceivedPacket()
    {
        std::lock_guard const lock(_mutex);
        if (_udpReceivedQueue.empty()) {
            return std::nullopt;
        }
        cmn::packetData data = _udpReceivedQueue.front();
        _udpReceivedQueue.pop();
        return data;
    }

    void ClientSharedData::addUdpPacketToSend(const cmn::CustomPacket &packet)
    {
        std::lock_guard const lock(_mutex);
        _udpSendQueue.push(packet);
    }

    std::optional<cmn::CustomPacket> ClientSharedData::getUdpPacketToSend()
    {
        std::lock_guard const lock(_mutex);
        if (_udpSendQueue.empty()) {
            return std::nullopt;
        }
        cmn::CustomPacket packet = _udpSendQueue.front();
        _udpSendQueue.pop();
        return packet;
    }

    void ClientSharedData::addTcpReceivedPacket(const cmn::packetData &data)
    {
        std::lock_guard const lock(_mutex);
        _tcpReceivedQueue.push(data);
    }

    std::optional<cmn::packetData> ClientSharedData::getTcpReceivedPacket()
    {
        std::lock_guard const lock(_mutex);
        if (_tcpReceivedQueue.empty()) {
            return std::nullopt;
        }
        cmn::packetData data = _tcpReceivedQueue.front();
        _tcpReceivedQueue.pop();
        return data;
    }

    void ClientSharedData::addTcpPacketToSend(const cmn::CustomPacket &packet)
    {
        std::lock_guard const lock(_mutex);
        _tcpSendQueue.push(packet);
    }

    std::optional<cmn::CustomPacket> ClientSharedData::getTcpPacketToSend()
    {
        std::lock_guard const lock(_mutex);
        if (_tcpSendQueue.empty()) {
            return std::nullopt;
        }
        cmn::CustomPacket packet = _tcpSendQueue.front();
        _tcpSendQueue.pop();
        return packet;
    }

}
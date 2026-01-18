/*
** EPITECH PROJECT, 2026
** R_Type
** File description:
** client_shared_data
*/

#include "ClientSharedData.hpp"

namespace client {

    void ClientSharedData::addUdpReceivedPacket(const cmn::packetData &packet)
    {
        std::lock_guard const lock(_mutex);
        _udpReceivedQueue.push(packet);
    }

    std::optional<cmn::packetData> ClientSharedData::getUdpReceivedPacket()
    {
        std::lock_guard const lock(_mutex);
        if (_udpReceivedQueue.empty()) {
            return std::nullopt;
        }
        cmn::packetData packet = _udpReceivedQueue.front();
        _udpReceivedQueue.pop();
        return packet;
    }

    void ClientSharedData::addUdpPacketToSend(const cmn::packetData &packet)
    {
        std::lock_guard const lock(_mutex);
        _udpSendQueue.push(packet);
    }

    std::optional<cmn::packetData> ClientSharedData::getUdpPacketToSend()
    {
        std::lock_guard const lock(_mutex);
        if (_udpSendQueue.empty()) {
            return std::nullopt;
        }
        cmn::packetData packet = _udpSendQueue.front();
        _udpSendQueue.pop();
        return packet;
    }

    void ClientSharedData::addTcpReceivedPacket(const cmn::packetData &packet)
    {
        std::lock_guard const lock(_mutex);
        _tcpReceivedQueue.push(packet);
    }

    std::optional<cmn::packetData> ClientSharedData::getTcpReceivedPacket()
    {
        std::lock_guard const lock(_mutex);
        if (_tcpReceivedQueue.empty()) {
            return std::nullopt;
        }
        cmn::packetData packet = _tcpReceivedQueue.front();
        _tcpReceivedQueue.pop();
        return packet;
    }

    void ClientSharedData::addTcpPacketToSend(const cmn::packetData &packet)
    {
        std::lock_guard const lock(_mutex);
        _tcpSendQueue.push(packet);
    }

    std::optional<cmn::packetData> ClientSharedData::getTcpPacketToSend()
    {
        std::lock_guard const lock(_mutex);
        if (_tcpSendQueue.empty()) {
            return std::nullopt;
        }
        cmn::packetData packet = _tcpSendQueue.front();
        _tcpSendQueue.pop();
        return packet;
    }

    void ClientSharedData::stopGame()
    {
        std::lock_guard const lock(_mutex);
        _isRunning = false;
    }

    bool ClientSharedData::isGameRunning()
    {
        std::lock_guard const lock(_mutex);
        return _isRunning;
    }

}
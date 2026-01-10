/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** PacketFactory
*/

#include "PacketFactory.hpp"
#include "constants/ProtocolConstants.hpp"
#include "constants/GameConstants.hpp"
#include "constants/BitPackingConstants.hpp"

namespace cmn {

    CustomPacket PacketFactory::createConnectionPacket(uint32_t playerId)
    {
        BitPacker packer;

        packer.writeUInt16(connectionProtocolId);
        packer.writeUInt32(playerId);

        auto buffer = packer.getBuffer();
        CustomPacket packet;

        for (auto &data : buffer) {
            packet << data;
        }
        return packet;
    }

    CustomPacket PacketFactory::createInputPacket(uint32_t playerId, Keys key, KeyState state)
    {
        BitPacker packer;

        packer.writeUInt16(inputProtocolId);
        packer.writeUInt32(playerId);
        packer.writeUInt8(static_cast<uint8_t>(key));
        packer.writeUInt8(static_cast<uint8_t>(state));

        auto buffer = packer.getBuffer();
        CustomPacket packet;

        for (auto &data : buffer) {
            packet << data;
        }
        return packet;
    }

    CustomPacket PacketFactory::createPositionPacket(std::pair<float, float> positionPair, uint64_t entityId)
    {
        BitPacker packer;

        packer.writeUInt16(positionProtocolId);
        packer.writeUInt32(entityId);
        packer.writeFloat(positionPair.first, 0, windowWidth, xPositionFloatPrecision);
        packer.writeFloat(positionPair.second, 0, windowHeight, yPositionFloatPrecision);

        auto buffer = packer.getBuffer();
        CustomPacket packet;

        for (auto &data : buffer) {
            packet << data;
        }
        return packet;
    }

    CustomPacket PacketFactory::createNewEntityPacket(EntityType type, std::pair<float, float> positionPair, uint64_t entityId)
    {
        BitPacker packer;

        packer.writeUInt16(newEntityProtocolId);
        packer.writeUInt32(entityId);
        packer.writeUInt8(static_cast<uint8_t>(type));
        packer.writeFloat(positionPair.first, 0, windowWidth, xPositionFloatPrecision);
        packer.writeFloat(positionPair.second, 0, windowHeight, yPositionFloatPrecision);

        auto buffer = packer.getBuffer();
        CustomPacket packet;

        for (auto &data : buffer) {
            packet << data;
        }
        return packet;
    }

    CustomPacket PacketFactory::createDeleteEntityPacket(uint64_t entityId)
    {
        BitPacker packer;

        packer.writeUInt16(deleteEntityProtocolId);
        packer.writeUInt32(entityId);

        auto buffer = packer.getBuffer();
        CustomPacket packet;

        for (auto &data : buffer) {
            packet << data;
        }
        return packet;
    }

    CustomPacket PacketFactory::createStartGamePacket()
    {
        BitPacker packer;

        packer.writeUInt16(startGameProtocolId);

        auto buffer = packer.getBuffer();
        CustomPacket packet;

        for (auto &data : buffer) {
            packet << data;
        }
        return packet;
    }

}// namespace cmn
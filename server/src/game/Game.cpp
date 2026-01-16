/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Game
*/

#include "Game.hpp"
#include "components/Collision.hpp"
#include "components/Enemy.hpp"
#include "components/Health.hpp"
#include "components/InputPlayer.hpp"
#include "components/Position.hpp"
#include "components/Shoot.hpp"
#include "components/Sound.hpp"
#include "constants/GameConstants.hpp"
#include "constants/GameConstants.hpp"
#include "data_translator/DataTranslator.hpp"
#include "entity_factory/EntityFactory.hpp"
#include "packet_data/PacketData.hpp"
#include "packet_disassembler/PacketDisassembler.hpp"
#include "packet_data/PacketData.hpp"
#include "packet_disassembler/PacketDisassembler.hpp"
#include "packet_factory/PacketFactory.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/DestroySystem.hpp"
#include "systems/HealthSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/ShootSystem.hpp"
#include "systems/VelocitySystem.hpp"
#include <algorithm>
#include <random>

namespace server {

     Game::Game(const std::shared_ptr<ServerSharedData> &data, int lobbyId, cmn::LobbyType type)
    : _sharedData(data), _lobbyId(lobbyId), _lobbyType(type)
     {
         try {
             ecs::EcsManager const manager {};
             _ecs = manager;
         } catch (std::exception &e) {
             throw std::exception();
         }
     }


    void Game::run()
    {
         //TODO: handle that private lobby can laucnh a new game inside the previous one
        _initLevels();
        _waitForPlayers();
        _sharedData->addLobbyTcpPacketToSend(_lobbyId, cmn::PacketFactory::createStartGamePacket());
        _sendPlayerEntities();
        _initEcsManager();
        _startGame();
    }

    void Game::_startGame()
    {
        Level &currentLevel = _levelManager.getCurrentLevel();
         // TODO: implement win loose -> implement death player mdr cest pas fait
        sf::Clock fpsClock;
        sf::Clock clock;
        sf::Clock enemyClock;
        unsigned const seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::minstd_rand0 generator(seed);
        float elapsedTime = 0.0F;
        constexpr float frameTimer = 0.016F;

        while (_sharedData->getLobbyState(_lobbyId) == cmn::LobbyState::Running) {
            float const deltaTime = clock.restart().asSeconds();
            std::optional<cmn::packetData> packet = _sharedData->getLobbyUdpReceivedPacket(_lobbyId);

            if (packet.has_value()) {
                    cmn::DataTranslator::translate(_ecs, packet.value(), _playerIdEntityMap);
            }
            _createEnemy(currentLevel, enemyClock, generator);
            _checkSpaceBar();
            if (elapsedTime > frameTimer) {
                fpsClock.restart();
                _sendPositions();
            }
            _sendSound();
            _sendDestroy();
            _ecs.setDeltaTime(deltaTime);
            _ecs.updateSystems();
            elapsedTime = fpsClock.getElapsedTime().asSeconds();
        }
    }

    void Game::_sendDestroy()
    {
        for (auto &entity : _ecs.getEntitiesWithComponent<ecs::Destroy>()) {
            _sharedData->addLobbyUdpPacketToSend(_lobbyId, cmn::PacketFactory::createDeleteEntityPacket(entity->getId()));
        }
    }

    void Game::_sendSound()
     {
         for (auto &entity : _ecs.getEntitiesWithComponent<ecs::Sound>()) {
             uint8_t soundId = static_cast<uint8_t>(entity->getComponent<ecs::Sound>()->getIdMusic());
             _sharedData->addLobbyUdpPacketToSend(_lobbyId,
                cmn::PacketFactory::createNewEntityPacket(
                    enemy.type,
                    position,
                    newEnemy->getId()
                )
            );
            enemy.lastSpawnTime = elapsed;
        }
    }

    if (elapsed >= waveDuration) {
        currentLevel.nextWave();
        enemyClock.restart();
    }
}

    void Game::_sendPlayerEntities()
    {
        for (auto &entity : _ecs.getEntities()) {
            auto component = entity->getComponent<ecs::Position>();
            std::pair<float, float> const pos = {component->getX(), component->getY()};

            _sharedData->addUdpPacketToSend(
                cmn::PacketFactory::createNewEntityPacket(
                    cmn::EntityType::Player,
                    pos,
                    entity->getId()
                )
            );
        }
    }

    void Game::_waitForPlayers()
    {
        size_t currentNbPlayerEntities = 0;
        auto listPlayerIds = _sharedData->getLobbyPlayers(_lobbyId);

         if (_lobbyType != cmn::LobbyType::Lobby) {
             _createNewPlayers(_sharedData->getLobbyPlayers(_lobbyId), currentNbPlayerEntities);
             return;
         }
        while (_readyPlayersId.size() != listPlayerIds.size() && _sharedData->getLobbyState(_lobbyId)) {
            listPlayerIds = _sharedData->getLobbyPlayers(_lobbyId);
            if (currentNbPlayerEntities != listPlayerIds.size()) {
                _createNewPlayers(listPlayerIds, currentNbPlayerEntities);
            }

            std::optional<cmn::packetData> packet = _sharedData->getLobbyUdpReceivedPacket(_lobbyId);;

            if (!packet.has_value()) {
                continue;
            }

            cmn::DataTranslator::translate(_ecs, packet.value(), _playerIdEntityMap);

            auto entities = _ecs.getEntitiesWithComponent<ecs::InputPlayer>();

            for (auto &entity : entities) {
                if (entity->getComponent<ecs::InputPlayer>()->getReady()
                    && !_isIdAlreadyPresent(_entityIdPlayerMap[entity->getId()])) {
                    _readyPlayersId.push_back(_entityIdPlayerMap[entity->getId()]);
                }
            }
        }
    }

    bool Game::_isIdAlreadyPresent(int playerId)
    {
        return std::ranges::any_of(
            _readyPlayersId,
            [playerId](int id) { return id == playerId; }
        );
    }

    void Game::_initLevels()
    {
        _levelManager.loadLevelFromFolder();
        _levelManager.setCurrentLevelId(1);
    }

    void Game::_createNewPlayers(const std::vector<int>& ids, size_t &currentNbPlayerEntities)
    {
        constexpr uint16_t playerPosX = 200;
        constexpr uint16_t playerPosY = 540;

        for (auto id : ids) {
            if (_playerIdEntityMap.contains(id)) {
                continue;
            }
            auto player =  cmn::EntityFactory::createEntity(_ecs,
                            cmn::EntityType::Player,
                            playerPosX, playerPosY,
                            cmn::EntityFactory::Context::SERVER);

            _playerIdEntityMap[id] = player->getId();
            _entityIdPlayerMap[player->getId()] = id;
            currentNbPlayerEntities++;
        }

    }

    void Game::_initEcsManager()
    {
        _ecs.addSystem<ecs::DestroySystem>();
        _ecs.addSystem<ecs::MovementSystem>();
        _ecs.addSystem<ecs::CollisionSystem>();
        _ecs.addSystem<ecs::ShootSystem>();
        _ecs.addSystem<ecs::VelocitySystem>();
        _ecs.addSystem<ecs::HealthSystem>();
    }

}
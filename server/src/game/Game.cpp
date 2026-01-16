/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Game
*/

#include "Game.hpp"
#include "components/Collision.hpp"
#include "components/InputPlayer.hpp"
#include "components/Position.hpp"
#include "components/Shoot.hpp"
#include "components/Sound.hpp"
#include "constants/GameConstants.hpp"
#include "data_translator/DataTranslator.hpp"
#include "entity_factory/EntityFactory.hpp"
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

     Game::Game(const std::shared_ptr<ServerSharedData> &data, const int lobbyId, const cmn::LobbyType lobbyType)
    : _sharedData(data), _lobbyId(lobbyId), _lobbyType(lobbyType)
     {
         try {
             ecs::EcsManager const manager {};
             _ecs = manager;
         } catch ([[maybe_unused]] std::exception &e) {
             throw std::exception();
         }
     }


    void Game::run()
    {
         //TODO: handle that private lobby can launch a new game inside the previous one
        _initLevels();
        _waitForPlayers();
        cmn::startGameData data = {};
        _sharedData->addLobbyTcpPacketToSend(_lobbyId, data);
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
            auto data = _sharedData->getLobbyUdpReceivedPacket(_lobbyId);

            if (data.has_value()) {
                cmn::DataTranslator::translate(_ecs, data.value(), _playerIdEntityMap);
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

    void Game::_sendDestroy() const
    {
        for (auto &entity : _ecs.getEntitiesWithComponent<ecs::Destroy>()) {
            cmn::deleteEntityData data = {entity->getId()};
            _sharedData->addLobbyUdpPacketToSend(_lobbyId, data);
        }
    }

    void Game::_sendSound() const
    {
         for (const auto &entity : _ecs.getEntitiesWithComponent<ecs::Sound>()) {
             uint8_t const soundId = static_cast<uint8_t>(entity->getComponent<ecs::Sound>()->getIdMusic());
             cmn::soundData data = {soundId};
             _sharedData->addLobbyUdpPacketToSend(_lobbyId, data);
             entity->removeComponent<ecs::Sound>();
         }
     }

    void Game::_sendPositions() const
     {
        for (const auto &entity : _ecs.getEntitiesWithComponent<ecs::Position>()) {
             const auto component = entity->getComponent<ecs::Position>();
            std::pair const position = { component->getX(), component->getY() };
            cmn::positionData data = {entity->getId(), position.first, position.second};
            _sharedData->addLobbyUdpPacketToSend(_lobbyId, data);
        }
    }

    void Game::_checkSpaceBar()
     {
         for (auto const &entity : _ecs.getEntitiesWithComponent<ecs::InputPlayer>()) {
             auto input = entity->getComponent<ecs::InputPlayer>();

             if (!entity->getComponent<ecs::Shoot>()) {
                 continue;
             }
             if (input) {
                 const auto shoot = entity->getComponent<ecs::Shoot>();

                 shoot->setTimeSinceLastShot(shoot->getTimeSinceLastShot() + _ecs.getDeltaTime());

                 if (input->getSpacebar()) {
                     if (shoot->getTimeSinceLastShot() >= shoot->getCooldown()) {
                         const auto positionCpn = entity->getComponent<ecs::Position>();
                         const auto collisionCpn = entity->getComponent<ecs::Collision>();

                         shoot->setTimeSinceLastShot(0);

                         float const posX = positionCpn->getX() + collisionCpn->getHeight();
                         float const posY = entity->getComponent<ecs::Position>()->getY();
                         auto shootCpn = entity->getComponent<ecs::Shoot>();

                         const auto projectile = cmn::EntityFactory::createEntity(_ecs,
                             cmn::EntityType::PlayerProjectile,
                             posX,
                             posY,
                             cmn::EntityFactory::Context::SERVER);

                        cmn::newEntityData data = {projectile->getId(), cmn::EntityType::PlayerProjectile, posX, posY};
                        _sharedData->addLobbyUdpPacketToSend(_lobbyId, data);
                    }
                }
            }
        }
    }

    void Game::_createEnemy(Level &currentLevel, sf::Clock &enemyClock, std::minstd_rand0 &generator)
    {
        if (currentLevel.isFinished()) {
            if (currentLevel.hasBossSpawned()) {
                return;
            }

            auto boss = currentLevel.getBoss();
            const auto newEnemy = cmn::EntityFactory::createEntity(_ecs,
                                cmn::EntityType::Boss1,
                                cmn::boss1SpawnPositionWidth, cmn::boss1SpawnPositionHeight,
                                cmn::EntityFactory::Context::SERVER, boss.second);


            cmn::newEntityData data = {newEnemy->getId(), cmn::EntityType::Boss1, cmn::boss1SpawnPositionWidth,
                cmn::boss1SpawnPositionHeight};

            _sharedData->addLobbyUdpPacketToSend(_lobbyId, data);
            currentLevel.setBossSpawned(true);
            return;
        }

        auto &waves = currentLevel.getWaves();
        int const waveId = currentLevel.getCurrentWaveId();

        if (waveId >= waves.size()) {
            return;
        }

        auto &[waveDuration, enemies] = waves[waveId];
        float const elapsed = enemyClock.getElapsedTime().asSeconds();

        for (auto &enemy : enemies) {
            if (elapsed - enemy.lastSpawnTime >= enemy.spawnRate) {
                const auto randNum = generator() % cmn::monsterMaxSpawnPositionHeight;
                const auto newEnemy =  cmn::EntityFactory::createEntity(_ecs,
                                cmn::EntityType::Plane,
                                cmn::monsterSpawnPositionWidth, static_cast<float>(randNum),
                                cmn::EntityFactory::Context::SERVER);


                std::pair<float, float> const position = {
                    cmn::monsterSpawnPositionWidth,
                    static_cast<float>(randNum)
                };

                cmn::newEntityData enemyData = {newEnemy->getId(), enemy.type, position.first, position.second};

                _sharedData->addLobbyUdpPacketToSend(_lobbyId, enemyData);
                enemy.lastSpawnTime = elapsed;
            }
        }

        if (elapsed >= static_cast<float>(waveDuration)) {
            currentLevel.nextWave();
            enemyClock.restart();
        }
    }

    void Game::_sendPlayerEntities()
    {
        for (const auto &entity : _ecs.getEntities()) {
            const auto component = entity->getComponent<ecs::Position>();
            std::pair<float, float> const pos = {component->getX(), component->getY()};
            cmn::newEntityData data = {entity->getId(), cmn::EntityType::Player, pos.first, pos.second};
            _sharedData->addLobbyUdpPacketToSend(_lobbyId, data);
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

            auto data = _sharedData->getLobbyUdpReceivedPacket(_lobbyId);

            if (!data.has_value()) {
                continue;
            }

            cmn::DataTranslator::translate(_ecs, data.value(), _playerIdEntityMap);

            auto entities = _ecs.getEntitiesWithComponent<ecs::InputPlayer>();

            for (const auto &entity : entities) {
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
            [playerId](const int id) { return id == playerId; }
        );
    }

    void Game::_initLevels()
    {
        _levelManager.loadLevelFromFolder();
        _levelManager.setCurrentLevelId(1);
    }

    void Game::_createNewPlayers(const std::vector<int>& ids, size_t &currentNbPlayerEntities)
    {

        for (auto id : ids) {
            constexpr uint16_t playerPosX = 200;
            constexpr uint16_t playerPosY = 540;
            if (_playerIdEntityMap.contains(id)) {
                continue;
            }
            const auto player =  cmn::EntityFactory::createEntity(_ecs,
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
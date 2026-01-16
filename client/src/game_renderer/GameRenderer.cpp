/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Game
*/

#include "GameRenderer.hpp"

#include "client/Client.hpp"
#include "components/Background.hpp"
#include "constants/GameConstants.hpp"
#include "enums/Key.hpp"
#include "enums/LobbyType.hpp"
#include "packet_disassembler/PacketDisassembler.hpp"
#include "packet_factory/PacketFactory.hpp"
#include "systems/BackgroundSystem.hpp"
#include "systems/DestroySystem.hpp"
#include "systems/InputSystem.hpp"
#include "systems/PlayerAnimationSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/SpriteAnimationSystem.hpp"
#include "systems/VelocitySystem.hpp"

#include <functional>

namespace client {

    GameRenderer::GameRenderer(const std::shared_ptr<ClientSharedData> &data) : _sharedData(data)
    {
        sf::Vector2u const vector = sf::VideoMode::getDesktopMode().size;
        _window = sf::RenderWindow(sf::VideoMode({vector.x, vector.y}), "R-Type");
    }

    void GameRenderer::_initEcsSystem()
    {
        _gameEcs.addSystem<ecs::InputSystem>();
        _gameEcs.addSystem<ecs::PlayerAnimationSystem>();
        _gameEcs.addSystem<ecs::SpriteAnimationSystem>();
        _gameEcs.addSystem<ecs::RenderSystem>(_window);
        _gameEcs.addSystem<ecs::DestroySystem>();
        _gameEcs.addSystem<ecs::VelocitySystem>();
        _gameEcs.addSystem<ecs::BackgroundSystem>();

        _menuEcs.addSystem<ecs::InputSystem>();
        _menuEcs.addSystem<ecs::RenderSystem>(_window);
        _menuEcs.addSystem<ecs::DestroySystem>();
        _menuEcs.addSystem<ecs::BackgroundSystem>();
    }

    void GameRenderer::_initKeyboard()
    {
        const auto gameKeyboard = _gameEcs.createEntity(4);
        gameKeyboard->addComponent<ecs::InputPlayer>();
        _gameKeyboard = gameKeyboard;

        const auto menuKeyboard = _menuEcs.createEntity(0);
        menuKeyboard->addComponent<ecs::InputPlayer>();
        _menuKeyboard = menuKeyboard;
    }

    void GameRenderer::_initBackground()
    {
        constexpr sf::Vector2f scale(1.0F, 1.0F);
        constexpr sf::Vector2f posZero(0.0F, 0.0F);
        constexpr sf::Vector2f posOne(1920, 0);
        constexpr sf::Vector2f posTwo(3840, 0);
        constexpr sf::Vector2f veloFirstBackground(10.0F, 0.5F);
        constexpr sf::Vector2f veloSecondBackground(20.0F, 0.5F);
        constexpr int sizeFistBackground = 1920;
        constexpr int sizeSecondBackground = 3840;
        const auto pathFistBackground = std::string("./assets/bg-stars.png");
        const auto pathSecondBackground = std::string("./assets/planets_background.png");
        constexpr uint8_t firstId = 0;
        constexpr uint8_t secondId = 1;
        constexpr uint8_t thirdId = 2;
        constexpr uint8_t fourId = 3;

        const auto background = _gameEcs.createEntity(firstId);
        background->addComponent<ecs::Position>(posZero.x, posZero.y);
        background->addComponent<ecs::Velocity>(veloFirstBackground.x, veloFirstBackground.y);
        background->addComponent<ecs::Sprite>(_gameEcs.getResourceManager().getTexture(pathFistBackground), scale);
        background->addComponent<ecs::Background>(sizeFistBackground);

        const auto backgroundNext = _gameEcs.createEntity(secondId);
        backgroundNext->addComponent<ecs::Position>(posOne.x, posOne.y);
        backgroundNext->addComponent<ecs::Velocity>(veloFirstBackground.x, veloFirstBackground.y);
        backgroundNext->addComponent<ecs::Sprite>(_gameEcs.getResourceManager().getTexture(pathFistBackground), scale);
        backgroundNext->addComponent<ecs::Background>(sizeFistBackground);

        const auto start = _gameEcs.createEntity(thirdId);
        start->addComponent<ecs::Position>(posZero.x, posZero.y);
        start->addComponent<ecs::Velocity>(veloSecondBackground.x, veloSecondBackground.y);
        start->addComponent<ecs::Sprite>(_gameEcs.getResourceManager().getTexture(pathSecondBackground), scale);
        start->addComponent<ecs::Background>(sizeSecondBackground);
        const auto startNext = _gameEcs.createEntity(fourId);
        startNext->addComponent<ecs::Position>(posTwo.x, posTwo.y);
        startNext->addComponent<ecs::Velocity>(veloSecondBackground.x, veloSecondBackground.y);
        startNext->addComponent<ecs::Sprite>(_gameEcs.getResourceManager().getTexture(pathSecondBackground), scale);
        startNext->addComponent<ecs::Background>(sizeSecondBackground);
    }


    void GameRenderer::_handleEvents()
    {
        while (const std::optional event = _window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                _window.close();
            }
        }
    }

    void GameRenderer::_checkGamePlayerInput()
    {
        static const std::array<
            std::pair<cmn::Keys, std::function<bool(const ecs::InputPlayer&)>>, 6
        > bindings = {{
            { cmn::Keys::Up,       [](const auto& keyboard){ return keyboard.getUp(); } },
            { cmn::Keys::Down,     [](const auto& keyboard){ return keyboard.getDown(); } },
            { cmn::Keys::Left,     [](const auto& keyboard){ return keyboard.getLeft(); } },
            { cmn::Keys::Right,    [](const auto& keyboard){ return keyboard.getRight(); } },
            { cmn::Keys::Space,    [](const auto& keyboard){ return keyboard.getSpacebar(); } },
            { cmn::Keys::R,         [](const auto& keyboard){ return keyboard.getReady(); } },
        }};

        const auto inputComp = _gameKeyboard->getComponent<ecs::InputPlayer>();

        bool isPressed = false;
        for (const auto& [key, check] : bindings) {
            if (check(*inputComp)) {
                _sharedData->addUdpPacketToSend(cmn::PacketFactory::createInputPacket(_playerId, key, cmn::KeyState::Pressed));
                isPressed = true;
            }
        }
        if (!isPressed) {
            _sharedData->addUdpPacketToSend(
                cmn::PacketFactory::createInputPacket(_playerId, cmn::Keys::None, cmn::KeyState::Pressed));
        }
    }

    void GameRenderer::_checkMenuPlayerInput() const
    {
        const auto inputComp = _menuKeyboard->getComponent<ecs::InputPlayer>();

        if (_currentState == ClientState::Menu) {
            if (inputComp->getOne()) {
                std::cout << "Ask to start solo mode with id: "<< _playerId << std::endl;
                _sharedData->addTcpPacketToSend(cmn::PacketFactory::createSelectModePacket(cmn::LobbyType::Solo, _playerId));
            } else if (inputComp->getTwo()) {
                _sharedData->addTcpPacketToSend(cmn::PacketFactory::createSelectModePacket(cmn::LobbyType::Matchmaking, _playerId));
            } else if (inputComp->getThree()) {
                _sharedData->addTcpPacketToSend(cmn::PacketFactory::createSelectModePacket(cmn::LobbyType::Lobby, _playerId));
            }
        }
        if (_currentState == ClientState::Waiting) {
            if (inputComp->getFour()) {
                _sharedData->addTcpPacketToSend(cmn::PacketFactory::createLeaveLobbyPacket(_playerId));
            }
        }
    }

    void GameRenderer::_updateNetwork()
    {
        static const std::unordered_map<int, uint64_t> emptyMap{};

        if (_currentState == ClientState::InGame) {
            while (auto packet = _sharedData->getUdpReceivedPacket()) {
                if (auto data = cmn::PacketDisassembler::disassemble(packet.value())) {
                    _translator.translate(_gameEcs, data.value(), emptyMap);
                }
            }
        } else {
            if (auto packet = _sharedData->getTcpReceivedPacket()) {
                if (auto data = cmn::PacketDisassembler::disassemble(packet.value())) {
                    std::visit(
                        [this](auto &&arg) {
                            using T = std::decay_t<decltype(arg)>;
                            if constexpr (std::is_same_v<T, cmn::connectionData>) {
                                _playerId = arg.playerId;
                                std::cout << "player id: " << _playerId << "\n";
                            } else if constexpr (std::is_same_v<T, cmn::startGameData>) {
                                _currentState = ClientState::InGame;
                                std::cout << "starting game\n";
                            } else if constexpr (std::is_same_v<T, cmn::joinLobbyData>) {
                                _currentState = ClientState::Waiting;
                                std::cout << "joining lobby" << "\n";
                            } else if constexpr (std::is_same_v<T, cmn::errorTcpData>) {
                                //TODO: implement error id which are in constant
                            }
                        },
                        data.value());
                }
            }
        }
    }

    void GameRenderer::_updateMenu(sf::Clock &inputClock, float elapsedTime, float deltaTime)
    {
        constexpr float inputCooldown = 0.1F;

        if (elapsedTime > inputCooldown) {
            _checkGamePlayerInput();
            inputClock.restart();
            elapsedTime = 0;
        }
        _checkMenuPlayerInput();
        _menuEcs.setDeltaTime(deltaTime);
        _menuEcs.updateSystems();
    }

    void GameRenderer::_updateGame(sf::Clock &inputClock, float elapsedTime, float deltaTime)
    {
        constexpr float inputCooldown = 0.016F;

        if (elapsedTime > inputCooldown) {
            _checkGamePlayerInput();
            inputClock.restart();
            elapsedTime = 0;
        }
        _gameEcs.setDeltaTime(deltaTime);
        _gameEcs.updateSystems();
    }

    void GameRenderer::run()
    {
        sf::Clock inputClock;
        float elapsedTime = 0;

        _initEcsSystem();
        _initBackground();
        _initKeyboard();
        while (_window.isOpen() && _sharedData->isGameRunning()) {
            const float deltaTime = _clock.restart().asSeconds();
            _updateNetwork();
            _handleEvents();
            switch (_currentState) {
                case ClientState::Menu:
                    _updateMenu(inputClock, elapsedTime, deltaTime);
                    break;
                case ClientState::Waiting:
                    _updateMenu(inputClock, elapsedTime, deltaTime);
                    break;
                case ClientState::InGame:
                    _updateGame(inputClock, elapsedTime, deltaTime);
                    break;
                case ClientState::GameOver:
                    //TODO: same and maybe win ?
                    _window.close();
                    return;
            }
            elapsedTime = inputClock.getElapsedTime().asSeconds();
        }
        _window.close();
    }


}
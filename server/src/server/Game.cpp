/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Game
*/

#include "Game.hpp"

#include "EcsManager.hpp"
#include "components/Animation.hpp"
#include "components/Health.hpp"
#include "components/PlayerAnimation.hpp"
#include "systems/DestroySystem.hpp"
#include "systems/HealthSystem.hpp"
#include "systems/PlayerAnimationSystem.hpp"
#include "systems/SpriteAnimationSystem.hpp"
constexpr int  SIZE_X_PLAYER = 33;
constexpr int SIZE_Y_PLAYER = 17;
void Game::run() {
    ecs::EcsManager _ecs;
    auto player = _ecs.createEntity();
    sf::RenderWindow window(sf::VideoMode({WINDOW_X, WINDOW_Y}), "R-Type");
    sf::Clock clock;
    sf::Clock enemyClock;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed);

    // auto background = _ecs.createEntity();
    // background->addComponent<ecs::Position>(0, 0);
    // sf::Vector2f scale(7.0f, 7.0f);
    // background->addComponent<ecs::Sprite>("./assets/bg-stars.png", scale);
    // background->addComponent<ecs::Animation>(std::pair<int, int>(272, 160), 0, 1);
    // background->addComponent<ecs::Velocity>(150, 0);

    player->addComponent<ecs::Health>(100);
    player->addComponent<ecs::Position>(200, WINDOW_Y / 2);
    player->addComponent<ecs::InputPlayer>();

    sf::Vector2f scale1(2.0f, 2.0f);
    player->addComponent<ecs::Sprite>("./assets/r-typesheet42.gif", scale1);
    player->addComponent<ecs::PlayerAnimation>();
    player->addComponent<ecs::Collision>(ecs::TypeCollision::PLAYER, SIZE_X_PLAYER, SIZE_Y_PLAYER);
    player->addComponent<ecs::Sound>("./sound/shoot.wav");
    player->addComponent<ecs::Shoot>(50, 0.5);

    _ecs.addSystem<ecs::InputSystem>();
    _ecs.addSystem<ecs::MovementSystem>();
    _ecs.addSystem<ecs::CollisionSystem>();
    _ecs.addSystem<ecs::ShootSystem>();
    _ecs.addSystem<ecs::PlayerAnimationSystem>();
    _ecs.addSystem<ecs::SpriteAnimationSystem>();
    _ecs.addSystem<ecs::RenderSystem>(window);
    _ecs.addSystem<ecs::VelocitySystem>();
    _ecs.addSystem<ecs::HealthSystem>();
    _ecs.addSystem<ecs::DestroySystem>();

    while (window.isOpen()) {
        float const deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        // -------- TODO remove that later - Dev and testing purpose only
        if (enemyClock.getElapsedTime().asSeconds() > 5) {
            enemyClock.restart();
            int const randNum = generator() % (WINDOW_Y + 1);
            auto newEnemy = _ecs.createEntity();

            newEnemy->addComponent<ecs::Position>(WINDOW_X + 100, randNum);
            newEnemy->addComponent<ecs::Enemy>();
            newEnemy->addComponent<ecs::Health>(100);
            newEnemy->addComponent<ecs::Sprite>("./assets/r-typesheet5.gif", scale1);
            newEnemy->addComponent<ecs::Animation>(std::pair<int, int>(32, 36), 0, 8);
            newEnemy->addComponent<ecs::Collision>(ecs::TypeCollision::ENEMY, 14, 18);
        }
        // --------
        _ecs.setDeltaTime(deltaTime);
        _ecs.updateSystems();
    }
}

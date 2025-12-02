/*
** EPITECH PROJECT, 2025
** r-type_client
** File description:
** RenderSystem
*/

#include "RenderSystem.hpp"
#include "InputPlayer.hpp"
#define SIZE_X_PLAYER 33
#define SIZE_Y_PLAYER 17

namespace ECS {
void RenderSystem::update(EcsManager &ecs)
{
    _window.clear(sf::Color::Black);
    const auto entities = ecs.getEntitiesWithComponent<Sprite>();
    const float dt = ecs.deltaTime();

    for (const auto& entity : entities) {
        auto spriteComp = entity->getComponent<Sprite>();
        auto positionComp = entity->getComponent<Position>();
        auto player = entity->getComponent<InputPlayer>();
        if (spriteComp) {
            auto sprite = spriteComp->getSprite();
            if (player) {
                renderPlayer(*player, dt, sprite);
            }
            //TODO
            //if (animatedSprite)
            if (positionComp) {
                sprite.setPosition({positionComp->getX(), positionComp->getY()});
            }
            _window.draw(sprite);
        }
    }
    _window.display();
}

void RenderSystem::renderPlayer(InputPlayer& player, const float dt, sf::Sprite& sprite)
{
    player.updateAnimation(dt);
    int const animFrame = player.getAnimFrame();
    const int playerId = player.getId();
    int x = 0;

    if (player.getUp()) {
        if (animFrame == 0) {
            x = 2 * SIZE_X_PLAYER;
        }
        else if (animFrame == 1) {
            x = 3 * SIZE_X_PLAYER;
        } else {
            x = 4 * SIZE_X_PLAYER;
        }
    } else if (player.getDown()) {
        if (animFrame == 0) {
            x = 2 * SIZE_X_PLAYER;
        }
        else if (animFrame == 1) {
            x = 1 * SIZE_X_PLAYER;
        } else {
            x = 0;
        }
    } else {
        x = 2 * SIZE_X_PLAYER;
    }
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(x, playerId * SIZE_Y_PLAYER),
        sf::Vector2i(SIZE_X_PLAYER, SIZE_Y_PLAYER)));
    sprite.setScale(sf::Vector2f(3,3));
}

} // namespace ECS

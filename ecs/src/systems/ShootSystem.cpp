/*
** EPITECH PROJECT, 2025
** r-type_client
** File description:
** ShootSystem
*/

#include "ShootSystem.hpp"
#include "components/Animation.hpp"

namespace ecs {
    /**
 * @brief Shoot a project
 * If the player touch the key space a projectile is send
 * @param ecs
     */
    void ShootSystem::update(EcsManager &ecs)
    {
        float dt = ecs.getDeltaTime();

        for (auto const &entity : ecs.getEntitiesWithComponent<Shoot>())
        {
            auto input = entity->getComponent<InputPlayer>();
            const auto shoot = entity->getComponent<Shoot>();
            const auto collision= entity->getComponent<Collision>();

            if (!shoot && !collision) { continue; }
            shoot->setTimeSinceLastShot(shoot->getTimeSinceLastShot() + ecs.getDeltaTime());
            if (input && input->getSpacebar()) {
                if (shoot->getTimeSinceLastShot() >= shoot->getCooldown())
                {
                    createEntity(ecs, entity, shoot, PLAYER_PROJECTILE);
                }
            }
            if (collision && collision->getTypeCollision() == ENEMY) {
                shoot->setShootTimer(shoot->getShootTimer() + dt);
                std::cout << "timer: " << shoot->getShootTimer() << std::endl;
                if (shoot->getShootTimer() >= 1.5f) {
                    shoot->setTimeSinceLastShot(0);
                    createEntity(ecs, entity, shoot, ENEMY_PROJECTILE);
                    shoot->setShootTimer(0.f);
                }
            }
            // if (input && input->getSpacebar()) {
            //     if (shoot->getTimeSinceLastShot() >= shoot->getCooldown() && (shoot->shotsLeft != 0))) {
            //         createEntity(ecs, entity, shoot, PLAYER_PROJECTILE);
            //     }
            // }
            // if (collision && collision->getTypeCollision() == ENEMY) {
            // if (shoot->getTimeSinceLastShot() >= shoot->getCooldown()) {
            // createEntity(ecs, entity, shoot, ENEMY_PROJECTILE);
        // }
        // }
        }
    }

    void ShootSystem::createEntity(EcsManager &ecs, std::shared_ptr<Entity> const &entity, std::shared_ptr<Shoot> shoot, TypeCollision type)
    {
        auto projectile = ecs.createEntity();
        const auto velocity= entity->getComponent<Velocity>();
        float vel = 400;
        int dir = (type == PLAYER_PROJECTILE) ? 1 : 0;

        if (!velocity && !projectile) return;

        if (velocity) {
            vel = velocity->getVelocity() * 2;
            dir = velocity->getDirection();
        }
        float spawnX = entity->getComponent<Position>()->getX();

        if (type == PLAYER_PROJECTILE) {
            spawnX += entity->getComponent<Collision>()->getWidth();
        } else {
            spawnX -= 10.f;
        }

        shoot->setTimeSinceLastShot(0);
        projectile->addComponent<ecs::Position>(
            spawnX,
            entity->getComponent<Position>()->getY()
        );
        projectile->addComponent<Velocity>(vel, dir);
        sf::Vector2f const scale(1.0f, 1.0f);
        projectile->addComponent<Sprite>(ecs.getResourceManager()
                .getTexture("./assets/r-typesheet30a.gif"), scale);
        projectile->addComponent<Animation>(std::pair<int, int>(34, 34), 0, 3);
        projectile->addComponent<Collision>(type, 10, 10);
        projectile->addComponent<Animation>(std::pair<int, int>(32, 36), 0, 3);
    }
}

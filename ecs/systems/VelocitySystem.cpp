/*
** EPITECH PROJECT, 2025
** r-type_client
** File description:
** VelocitySystem
*/

#include "VelocitySystem.hpp"

namespace ECS {

    void VelocitySystem::update(EcsManager &ecs)
    {
        float dt = ecs.deltaTime();

        for (auto const &entity : ecs.getEntitiesWithComponent<Velocity>()) {
            auto pos = entity->getComponent<Position>();
            auto direction = entity->getComponent<Velocity>()->getDirection();
//            if (input) {
                if (direction == 0) {
                    pos->setY(pos->getY() - (10 * dt));
                }
                if (direction == 1) {
                    pos->setY(pos->getY() + (10 * dt));
                }
                if (direction == 2) {
                    pos->setX(pos->getX() - (10 * dt));
                }
                if (direction == 3) {
                    pos->setX(pos->getX() + (10 * dt));
                }
//            }
        }

    }
}

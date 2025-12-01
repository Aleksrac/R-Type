/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** InputSystem
*/

#include "InputSystem.hpp"

void InputSystem::update(EcsManager & ecs) {
    for (auto& entity : ecs.getEntities()) {
        auto inputPtr = entity->getComponent<InputPlayer>();
        if (!inputPtr) continue;
        inputPtr->setDown(sf::Keyboard::isKeyPressed(sf::Keyboard::S));
        inputPtr->setUp(sf::Keyboard::isKeyPressed(sf::Keyboard::W));
        inputPtr->setLeft(sf::Keyboard::isKeyPressed(sf::Keyboard::A));
        inputPtr->setRight(sf::Keyboard::isKeyPressed(sf::Keyboard::D));
    }
}
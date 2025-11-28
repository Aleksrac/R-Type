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

        inputPtr->_up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        inputPtr->_down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        inputPtr->_left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        inputPtr->_right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
    }
}
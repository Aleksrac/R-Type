/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Sprite
*/


#include "Sprite.hpp"

Sprite::Sprite(const std::string &sprite)
{
    if (!_texture.loadFromFile(sprite)) {
        throw std::runtime_error("Failed to load sprite: " + sprite);
    }
    _sprite.setTexture(_texture);
}

const sf::Sprite& Sprite::getSprite() const{
    return _sprite;
}

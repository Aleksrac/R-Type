/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Sprite
*/


#ifndef R_TYPE_SPRITE_HPP
#define R_TYPE_SPRITE_HPP
#include "Component.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace ECS {
class Sprite : public Component
{
  public:
    explicit Sprite(const std::string &texturePath, bool const animated);
    ~Sprite() override = default;
    const sf::Sprite &getSprite() const;
    const bool isAnimated() const;
  private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    bool _isAnimated = false;
};
}



#endif //R_TYPE_SPRITE_HPP

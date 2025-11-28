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


class Sprite : public Component
{
    public:
        explicit Sprite(const std::string &sprite); 
        ~Sprite() override = default;
        const sf::Sprite &getSprite() const; 
    private:
        sf::Sprite _sprite;
        sf::Texture _texture;

};



#endif //R_TYPE_SPRITE_HPP

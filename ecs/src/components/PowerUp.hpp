/*
** EPITECH PROJECT, 2026
** R_Type
** File description:
** PowerUp
*/

#ifndef R_TYPE_POWERUP_HPP
#define R_TYPE_POWERUP_HPP
#include "Component.hpp"
namespace ecs {
    class PowerUp : public Component
    {
        public:
        PowerUp() = default;
        ~PowerUp() = default;
    };
    }
#endif// R_TYPE_POWERUP_HPP

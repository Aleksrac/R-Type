/*
** EPITECH PROJECT, 2025
** r-type_client
** File description:
** PlayerAnimationSystem
*/

#ifndef R_TYPE_CLIENT_PLAYERANIMATIONSYSTEM_HPP
#define R_TYPE_CLIENT_PLAYERANIMATIONSYSTEM_HPP
#include "System.hpp"
#include "components/Animation.hpp"
#include "EcsManager.hpp"
#include "components/InputPlayer.hpp"
#include "components/Sprite.hpp"

#define SIZE_X_PLAYER 33
#define SIZE_Y_PLAYER 17

namespace ecs {
class PlayerAnimationSystem : public System
{
    public:
        PlayerAnimationSystem() = default;
        ~PlayerAnimationSystem() override = default;
        void update(EcsManager &ecs) override;

};
}



#endif// R_TYPE_CLIENT_PLAYERANIMATIONSYSTEM_HPP

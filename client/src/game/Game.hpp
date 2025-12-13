/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Game
*/

#ifndef R_TYPE_GAME_HPP
#define R_TYPE_GAME_HPP
#define WINDOW_X 1920
#define WINDOW_Y 1080
#include "EcsManager.hpp"
#include "shared_data/SharedData.hpp"

namespace client {
    class Game {
    public:
        Game(std::shared_ptr<cmn::SharedData>);
        ~Game() = default;
        [[noreturn]] static void run();
    private:
        ecs::EcsManager _ecs;
        std::shared_ptr<cmn::SharedData> _sharedData;
    };
}


#endif //R_TYPE_GAME_HPP

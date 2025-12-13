/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** Game
*/

#include "Game.hpp"

#include "client/Client.hpp"

namespace client {

     Game::Game(std::shared_ptr<cmn::SharedData> data):
        _ecs(ecs::EcsManager()), _sharedData(std::move(data)) {}

    void Game::run()
    {
        while (true) {

        }
    }

}
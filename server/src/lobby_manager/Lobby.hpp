/*
** EPITECH PROJECT, 2026
** R_Type
** File description:
** Lobby
*/

#ifndef R_TYPE_LOBBY_HPP
#define R_TYPE_LOBBY_HPP
#include "enums/LobbyState.hpp"
#include "enums/LobbyType.hpp"
#include <chrono>

namespace server {
    using Lobby = struct Lobby {
        int id;
        cmn::LobbyType type;
        cmn::LobbyState state;
        int code;
        std::chrono::steady_clock::time_point createdAt;
    };
}

#endif// R_TYPE_LOBBY_HPP

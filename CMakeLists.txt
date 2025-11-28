cmake_minimum_required(VERSION 3.10)

project(R_Type)

set(CMAKE_CXX_STANDARD 14)

include_directories(. GameEngine/Components GameEngine/Systems GameEngine/)

add_executable(R_Type
        main.cpp
        main.hpp
        GameEngine/Systems/System.cpp
        GameEngine/Systems/System.hpp
        GameEngine/Systems/RenderSystem.cpp
        GameEngine/Systems/RenderSystem.hpp
        GameEngine/Systems/MovementSystem.cpp
        GameEngine/Systems/MovementSystem.hpp
        GameEngine/Systems/InputSystem.cpp
        GameEngine/Systems/InputSystem.hpp
        GameEngine/Components/Sound.cpp
        GameEngine/Components/Sound.hpp
        GameEngine/Components/Collider.cpp
        GameEngine/Components/Collider.hpp
        GameEngine/Components/InputPlayer.cpp
        GameEngine/Components/InputPlayer.hpp
        GameEngine/Components/Health.cpp
        GameEngine/Components/Health.hpp
        GameEngine/Components/Position.cpp
        GameEngine/Components/Position.hpp
        GameEngine/Components/Component.cpp
        GameEngine/Components/Component.hpp
        GameEngine/Game.hpp
        GameEngine/Game.cpp
        GameEngine/Entity.cpp
        GameEngine/Entity.hpp
        GameEngine/EcsManager.cpp
        GameEngine/EcsManager.hpp
        GameEngine/Components/Sprite.cpp
        GameEngine/Components/Sprite.hpp
        GameEngine/Components/Sprite.hpp
        GameEngine/Components/Enemy.cpp
        GameEngine/Components/Enemy.hpp)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

target_link_libraries(R_Type PRIVATE sfml-graphics sfml-window sfml-system)

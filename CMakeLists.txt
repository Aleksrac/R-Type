cmake_minimum_required(VERSION 3.10)

project(r-type_client)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)
include_directories(. ecs/components ecs/systems ecs/)

add_subdirectory(GameEngine)

#add_executable(R_Type
#        main.cpp
#        main.hpp
#)

#target_link_libraries(R_Type PRIVATE
#        sfml-graphics
#        sfml-window
#        sfml-system
#)

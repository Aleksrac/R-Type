add_library(r-type_server_lib
        parser/Parser.cpp
        parser/Parser.hpp
        server/Server.cpp
        server/Server.hpp
        game/Game.cpp
        game/Game.hpp
        level/Level.cpp
        level/Level.hpp
        level_manager/LevelManager.cpp
        level_manager/LevelManager.hpp
        parser/LevelParser.cpp
        parser/LevelParser.hpp
        level/TmpEnemy.hpp
)
target_link_libraries(r-type_server_lib PRIVATE config++)
target_include_directories(r-type_server_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(r-type_server_lib
        PUBLIC
        SFML::Network
        r-type_ecs_lib
        r-type_common_lib
)

add_executable(r-type_server
        main.cpp
        parser/LevelParser.cpp
        parser/LevelParser.hpp
        parser/Parser.cpp
        parser/Parser.hpp
        server/Server.cpp
        server/Server.hpp
        game/Game.cpp
        game/Game.hpp
        level/Level.cpp
        level/Level.hpp
        level_manager/LevelManager.cpp
        level_manager/LevelManager.hpp
)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

target_include_directories(r-type_server PRIVATE ${LIBCONFIG_INCLUDE_DIRS})
target_link_libraries(r-type_server PRIVATE ${LIBCONFIG_LIBRARIES})

target_link_libraries(r-type_server PRIVATE r-type_server_lib r-type_ecs_lib r-type_common_lib)
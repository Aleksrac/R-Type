/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** LevelManager
*/

#include "LevelManager.hpp"
#include "parser/LevelParser.hpp"
#include <filesystem>

namespace server {

    void LevelManager::addLevel(Level &level)
    {
        _levels.push_back(level);
    }

    void LevelManager::loadLevelFromFolder()
    {
        LevelParser parser;

        for (const auto& entry : std::filesystem::directory_iterator(cmn::folderLevels)) {
            if (std::filesystem::is_regular_file(entry.path())) {
                const Level level = parser.createLevel(cmn::folderLevels + '/' + entry.path().filename().string());
                _levels.push_back(level);
            }
        }
    }

    Level &LevelManager::getCurrentLevel()
    {
        for (auto &level : _levels) {
            if (level.getLevelId() == _currentLevelId) {
                return level;
            }
        }
        throw std::exception();
    }

    void LevelManager::setCurrentLevelId(uint8_t id)
    {
        _currentLevelId = id;
    }

    void LevelManager::changeToNextLevel()
    {
        if (_currentLevelId == _levels.end()->getLevelId()) {
            throw std::exception();
        }

        uint8_t previousLevelId = 0;

        for (auto &level : _levels) {
            if (previousLevelId == _currentLevelId) {
                _currentLevelId = level.getLevelId();
                return;
            }
            previousLevelId = level.getLevelId();
        }
    }

}// namespace server
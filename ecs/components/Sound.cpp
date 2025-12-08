/*
** EPITECH PROJECT, 2025
** r-type_client
** File description:
** Sound
*/

#include <iostream>
#include "Sound.hpp"

namespace ECS {
    Sound::Sound(const std::string& filepath)
    {
        if (!_buffer.loadFromFile(filepath))
            throw std::runtime_error("Failed to load sound: " + filepath);
        _sound = std::make_unique<sf::Sound>(_buffer);
        _sound->setVolume(100.f);
    }

    const sf::Sound& Sound::getSound() const
    {
        return *_sound;
    }

    void Sound::play()
    {
        if (_sound) {
            if (_sound->getStatus() == sf::Sound::Status::Playing) {
                _sound->stop();
            }
            _sound->play();
        }

    }

}
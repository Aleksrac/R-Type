/*
** EPITECH PROJECT, 2025
** R_Type
** File description:
** DataTranslator
*/

#include "DataTranslator.hpp"
#include "Constants.hpp"
#include "components/Animation.hpp"
#include "components/Destroy.hpp"
#include "components/InputPlayer.hpp"
#include "components/PlayerAnimation.hpp"
#include "components/Position.hpp"
#include "components/Sound.hpp"
#include "components/Sprite.hpp"
#include "enums/EntityType.hpp"
#include "enums/Key.hpp"
#include <list>

namespace cmn {

    const std::list<std::function<void(Keys, std::shared_ptr<ecs::InputPlayer>)>> functionList = {
        [](Keys key,std::shared_ptr<ecs::InputPlayer> component) {key == Keys::Down ? component->setDown(true) : component->setDown(false);},
        [](Keys key,std::shared_ptr<ecs::InputPlayer> component) {key == Keys::Up ? component->setUp(true) : component->setUp(false);},
        [](Keys key,std::shared_ptr<ecs::InputPlayer> component) {key == Keys::Left ? component->setLeft(true) : component->setLeft(false);},
        [](Keys key,std::shared_ptr<ecs::InputPlayer> component) {key == Keys::Right ? component->setRight(true) : component->setRight(false);},
        [](Keys key,std::shared_ptr<ecs::InputPlayer> component) {key == Keys::Space ? component->setSpacebar(true) : component->setSpacebar(false);},
        [](Keys key,std::shared_ptr<ecs::InputPlayer> component) {key == Keys::R ? component->setR(true) : component->setR(false);},

    };

    void DataTranslator::_injectInput(ecs::EcsManager &ecs, inputPacket &input, int id)
    {
        for (auto &entity : ecs.getEntitiesWithComponent<ecs::InputPlayer>()) {
            if (entity->getId() == id) {
                auto component = entity->getComponent<ecs::InputPlayer>();
                Keys key = static_cast<Keys>(input.key);
                for (auto &function : functionList) {
                    function(key, component);
                }
                break;
            }
        }
    }

    void DataTranslator::_injectPosition(ecs::EcsManager &ecs, positionPacket &position, int id)
    {
        for (auto &entity : ecs.getEntitiesWithComponent<ecs::Position>()) {
            if (entity->getId() == id) {
                auto component = entity->getComponent<ecs::Position>();
                component->setX(position.posX);
                component->setY(position.posY);
                break;
            }
        }
    }

    void DataTranslator::_injectNewEntity(ecs::EcsManager &ecs, newEntityPacket &newEntity, int id)
    {
        auto entity = ecs.createEntity(id);

        if (static_cast<EntityType>(newEntity.type) == EntityType::YourPlayer) {
            _yourPlayerEntityId = id;
        }
        entity->addComponent<ecs::Position>(newEntity.posX, newEntity.posY);
        if (static_cast<EntityType>(newEntity.type) == EntityType::Player) {
            entity->addComponent<ecs::Sprite>(std::string(playerSpriteSheet), playerSpriteScale);
            entity->addComponent<ecs::PlayerAnimation>();
            entity->addComponent<ecs::Sound>(std::string(playerShootSound));
        }
        if (static_cast<EntityType>(newEntity.type) == EntityType::Monster) {
            entity->addComponent<ecs::Sprite>(std::string(monsterSpriteSheet), monsterSpriteScale);
            entity->addComponent<ecs::Animation>(monsterAnimationSize, monsterAnimationOffset, monsterAnimationNumberFrame);
        }
        if (static_cast<EntityType>(newEntity.type) == EntityType::PlayerProjectile) {
            entity->addComponent<ecs::Sprite>(std::string(playerProjectileSpriteSheet), playerProjectileScale);
            entity->addComponent<ecs::Animation>(playerProjectileAnimationSize, playerProjectileAnimationOffset, playerProjectileAnimationNumberFrame);
        }
    }

    void DataTranslator::_deleteEntity(ecs::EcsManager &ecs, deleteEntityPacket &deleteEntity, int id)
    {
        for (auto &entity : ecs.getEntitiesWithComponent<ecs::Position>()) {
            if (entity->getId() == id) {
                entity->addComponent<ecs::Destroy>();
                std::cout << "[EASTER_EGG]: " << deleteEntity.easterEgg << "\n";
                break;
            }
        }
    }

    void DataTranslator::translate(ecs::EcsManager &ecs, packetData &data)
    {
        std::visit([data, this, &ecs](auto &&arg)
            {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, inputPacket>) {
                    inputPacket &input = arg;
                    _injectInput(ecs, input, data.entityId);
                } else if constexpr (std::is_same_v<T, positionPacket>) {
                    positionPacket &position = arg;
                    _injectPosition(ecs, position, data.entityId);
                } else if constexpr (std::is_same_v<T, newEntityPacket>) {
                    newEntityPacket &newEntity = arg;
                    _injectNewEntity(ecs, newEntity, data.entityId);
                } else if constexpr (std::is_same_v<T, deleteEntityPacket>) {
                    deleteEntityPacket &deleteEntity = arg;
                    _deleteEntity(ecs, deleteEntity, data.entityId);
                }
            }, data.content);
    }

}// namespace cmn
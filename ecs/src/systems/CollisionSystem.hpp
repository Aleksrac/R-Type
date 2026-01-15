/*
** EPITECH PROJECT, 2025
** r-type_client
** File description:
** CollisionSystem
*/

#ifndef BOOTSTRAP_COLLISIONSYSTEM_HPP
    #define BOOTSTRAP_COLLISIONSYSTEM_HPP
#include "components/Sprite.hpp"
#include "EcsManager.hpp"
#include "components/Collision.hpp"
#include "components/Position.hpp"
#include "src/constants/GameConstants.hpp"

namespace ecs {
    struct AABB {
        float x, y;
        float width, height;

        bool intersects(const AABB& other) const {
            return !(x + width < other.x || other.x + other.width < x ||
                y + height < other.y || other.y + other.height < y);
        }
        bool contains(const AABB& other) const {
            return other.x >= x && other.x + other.width <= x + width &&
                other.y >= y && other.y + other.height <= y + height;
        }
    };
    using EntityRef = std::shared_ptr<Entity>;

    class QuadTree {
    public:
        QuadTree(const AABB& bound, const int depth = 0): _bound(bound), _depth(depth) {};
        ~QuadTree() = default;

        void insert(EntityRef entity, AABB &bound);
        std::vector<EntityRef> getEntities(const AABB&) const;

    private:
        void subdivide();

        static constexpr int MAX_ENTITIES = 6;
        static constexpr int MAX_DEPTH = 6;

        AABB _bound;
        int _depth;

        std::vector<std::pair<EntityRef, AABB>> _entities;

        std::unique_ptr<QuadTree> _northWest;
        std::unique_ptr<QuadTree> _northEast;
        std::unique_ptr<QuadTree> _southWest;
        std::unique_ptr<QuadTree> _southEast;
    };

    class CollisionSystem: public ecs::System {
      public:
        CollisionSystem() = default;
        void update(ecs::EcsManager &ecs) override;

    private:
        static bool shouldIgnoreCollision(ecs::TypeCollision a, ecs::TypeCollision b);
        static bool isColliding( float x1, float y1, float w1, float h1,
                                 float x2, float y2, float w2, float h2);
        static bool checkCollision(ecs::EcsManager& ecs, ecs::Entity a,ecs:: Entity b);
        void buildQuadTree(const std::vector<std::shared_ptr<Entity>> &entites);

        std::unique_ptr<ecs::QuadTree> _quadTree;
    };
}

#endif //BOOTSTRAP_COLLISIONSYSTEM_HPP
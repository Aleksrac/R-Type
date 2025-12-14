---
title: ecs::CollisionSystem

---

# ecs::CollisionSystem






`#include <CollisionSystem.hpp>`

Inherits from [ecs::System](Classes/classecs_1_1System.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| virtual void | **[update](Classes/classecs_1_1CollisionSystem.md#function-update)**([EcsManager](Classes/classecs_1_1EcsManager.md) & ecs) override |
| | **[CollisionSystem](Classes/classecs_1_1CollisionSystem.md#function-collisionsystem)**() =default |

## Additional inherited members

**Public Functions inherited from [ecs::System](Classes/classecs_1_1System.md)**

|                | Name           |
| -------------- | -------------- |
| virtual | **[~System](Classes/classecs_1_1System.md#function-~system)**() =default |
| | **[System](Classes/classecs_1_1System.md#function-system)**() =default |


## Public Functions Documentation

### function update

```cpp
virtual void update(
    EcsManager & ecs
) override
```


**Parameters**: 

  * **ecs** 


**Reimplements**: [ecs::System::update](Classes/classecs_1_1System.md#function-update)


Check if the entity can have a collision, and is there is one check if there is a collision with an another entity 


### function CollisionSystem

```cpp
CollisionSystem() =default
```


-------------------------------

Updated on 2025-12-14 at 20:10:14 +0000
---
title: ecs::EcsManager

---

# ecs::EcsManager






`#include <EcsManager.hpp>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[~EcsManager](Classes/classecs_1_1EcsManager.md#function-~ecsmanager)**() =default |
| void | **[updateSystems](Classes/classecs_1_1EcsManager.md#function-updatesystems)**() |
| void | **[setDeltaTime](Classes/classecs_1_1EcsManager.md#function-setdeltatime)**(float dt) |
| template <typename T \> <br>std::vector< std::shared_ptr< [Entity](Classes/classecs_1_1Entity.md) > > | **[getEntitiesWithComponent](Classes/classecs_1_1EcsManager.md#function-getentitieswithcomponent)**() const |
| std::vector< std::shared_ptr< [Entity](Classes/classecs_1_1Entity.md) > > & | **[getEntities](Classes/classecs_1_1EcsManager.md#function-getentities)**() |
| float | **[getDeltaTime](Classes/classecs_1_1EcsManager.md#function-getdeltatime)**() const |
| std::shared_ptr< [Entity](Classes/classecs_1_1Entity.md) > | **[createEntity](Classes/classecs_1_1EcsManager.md#function-createentity)**() |
| template <typename T ,typename... Args\> <br>std::shared_ptr< T > | **[addSystem](Classes/classecs_1_1EcsManager.md#function-addsystem)**(Args &&... args) |
| template <typename T \> <br>std::shared_ptr< T > | **[addSystem](Classes/classecs_1_1EcsManager.md#function-addsystem)**() |
| | **[EcsManager](Classes/classecs_1_1EcsManager.md#function-ecsmanager)**() =default |

## Public Functions Documentation

### function ~EcsManager

```cpp
~EcsManager() =default
```


### function updateSystems

```cpp
void updateSystems()
```


### function setDeltaTime

```cpp
void setDeltaTime(
    float dt
)
```


### function getEntitiesWithComponent

```cpp
template <typename T >
inline std::vector< std::shared_ptr< Entity > > getEntitiesWithComponent() const
```


### function getEntities

```cpp
std::vector< std::shared_ptr< Entity > > & getEntities()
```


### function getDeltaTime

```cpp
float getDeltaTime() const
```


### function createEntity

```cpp
std::shared_ptr< Entity > createEntity()
```


### function addSystem

```cpp
template <typename T ,
typename... Args>
inline std::shared_ptr< T > addSystem(
    Args &&... args
)
```


### function addSystem

```cpp
template <typename T >
inline std::shared_ptr< T > addSystem()
```


### function EcsManager

```cpp
EcsManager() =default
```


-------------------------------

Updated on 2025-12-13 at 17:56:13 +0000
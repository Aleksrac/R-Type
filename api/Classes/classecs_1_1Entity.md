---
title: ecs::Entity

---

# ecs::Entity






`#include <Entity.hpp>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[~Entity](Classes/classecs_1_1Entity.md#function-~entity)**() =default |
| template <typename T \> <br>std::shared_ptr< T > | **[getComponent](Classes/classecs_1_1Entity.md#function-getcomponent)**() |
| template <typename T ,typename... Args\> <br>std::shared_ptr< T > | **[addComponent](Classes/classecs_1_1Entity.md#function-addcomponent)**(Args &&... args) |
| template <typename T \> <br>std::shared_ptr< T > | **[addComponent](Classes/classecs_1_1Entity.md#function-addcomponent)**() |
| | **[Entity](Classes/classecs_1_1Entity.md#function-entity)**() =default |

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| size_t | **[_id](Classes/classecs_1_1Entity.md#variable--id)**  |

## Public Functions Documentation

### function ~Entity

```cpp
~Entity() =default
```


### function getComponent

```cpp
template <typename T >
inline std::shared_ptr< T > getComponent()
```


### function addComponent

```cpp
template <typename T ,
typename... Args>
inline std::shared_ptr< T > addComponent(
    Args &&... args
)
```


### function addComponent

```cpp
template <typename T >
inline std::shared_ptr< T > addComponent()
```


### function Entity

```cpp
Entity() =default
```


## Public Attributes Documentation

### variable _id

```cpp
size_t _id;
```


-------------------------------

Updated on 2025-12-13 at 18:06:21 +0000
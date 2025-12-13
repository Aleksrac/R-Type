---
title: ecs::Sprite

---

# ecs::Sprite






`#include <Sprite.hpp>`

Inherits from [ecs::Component](Classes/classecs_1_1Component.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[~Sprite](Classes/classecs_1_1Sprite.md#function-~sprite)**() override =default |
| void | **[setTextureRect](Classes/classecs_1_1Sprite.md#function-settexturerect)**(int x, int y, int width, int height) |
| const sf::Sprite & | **[getSprite](Classes/classecs_1_1Sprite.md#function-getsprite)**() const |
| sf::Vector2f | **[getScale](Classes/classecs_1_1Sprite.md#function-getscale)**() const |
| | **[Sprite](Classes/classecs_1_1Sprite.md#function-sprite)**(const std::string & texturePath, sf::Vector2f scale) |

## Additional inherited members

**Public Functions inherited from [ecs::Component](Classes/classecs_1_1Component.md)**

|                | Name           |
| -------------- | -------------- |
| virtual | **[~Component](Classes/classecs_1_1Component.md#function-~component)**() =default |
| | **[Component](Classes/classecs_1_1Component.md#function-component)**() =default |


## Public Functions Documentation

### function ~Sprite

```cpp
~Sprite() override =default
```


### function setTextureRect

```cpp
void setTextureRect(
    int x,
    int y,
    int width,
    int height
)
```


### function getSprite

```cpp
const sf::Sprite & getSprite() const
```


### function getScale

```cpp
sf::Vector2f getScale() const
```


### function Sprite

```cpp
explicit Sprite(
    const std::string & texturePath,
    sf::Vector2f scale
)
```


-------------------------------

Updated on 2025-12-13 at 18:06:21 +0000
---
title: server::Level

---

# server::Level






`#include <Level.hpp>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| uint8_t | **[getLevelId](Classes/classserver_1_1Level.md#function-getlevelid)**() const |
| bool | **[getIsBossPresent](Classes/classserver_1_1Level.md#function-getisbosspresent)**() const |
| std::string | **[getEnemySpritePath](Classes/classserver_1_1Level.md#function-getenemyspritepath)**() const |
| uint8_t | **[getEnemySpawnRate](Classes/classserver_1_1Level.md#function-getenemyspawnrate)**() const |
| std::string | **[getBossSpritePath](Classes/classserver_1_1Level.md#function-getbossspritepath)**() const |
| uint32_t | **[getBossApparitionTime](Classes/classserver_1_1Level.md#function-getbossapparitiontime)**() const |
| | **[Level](Classes/classserver_1_1Level.md#function-level)**(uint8_t levelId, std::string enemySpritePath, uint8_t enemySpawnRate, bool isBossPresent, std::string bossSpritePath, uint32_t bossApparitionTime) |

## Public Functions Documentation

### function getLevelId

```cpp
uint8_t getLevelId() const
```


### function getIsBossPresent

```cpp
bool getIsBossPresent() const
```


### function getEnemySpritePath

```cpp
std::string getEnemySpritePath() const
```


### function getEnemySpawnRate

```cpp
uint8_t getEnemySpawnRate() const
```


### function getBossSpritePath

```cpp
std::string getBossSpritePath() const
```


### function getBossApparitionTime

```cpp
uint32_t getBossApparitionTime() const
```


### function Level

```cpp
Level(
    uint8_t levelId,
    std::string enemySpritePath,
    uint8_t enemySpawnRate,
    bool isBossPresent,
    std::string bossSpritePath,
    uint32_t bossApparitionTime
)
```


-------------------------------

Updated on 2025-12-13 at 17:56:13 +0000
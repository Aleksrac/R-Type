---
title: server::Server

---

# server::Server






`#include <Server.hpp>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| int | **[sendUdp](Classes/classserver_1_1Server.md#function-sendudp)**(const cmn::packetData & packet, const sf::IpAddress & clientIp, uint16_t port) |
| int | **[sendTcp](Classes/classserver_1_1Server.md#function-sendtcp)**(const cmn::packetData & packet, sf::TcpSocket & clientSocket) |
| void | **[run](Classes/classserver_1_1Server.md#function-run)**() |
| void | **[close](Classes/classserver_1_1Server.md#function-close)**() |
| int | **[broadcastUdp](Classes/classserver_1_1Server.md#function-broadcastudp)**(const cmn::packetData & packet, uint16_t port) |
| int | **[broadcastTcp](Classes/classserver_1_1Server.md#function-broadcasttcp)**(const cmn::packetData & packet) const |
| int | **[bindPorts](Classes/classserver_1_1Server.md#function-bindports)**(uint16_t port) |
| | **[Server](Classes/classserver_1_1Server.md#function-server)**(const std::shared_ptr< cmn::SharedData > & data) |

## Public Functions Documentation

### function sendUdp

```cpp
int sendUdp(
    const cmn::packetData & packet,
    const sf::IpAddress & clientIp,
    uint16_t port
)
```


### function sendTcp

```cpp
static int sendTcp(
    const cmn::packetData & packet,
    sf::TcpSocket & clientSocket
)
```


### function run

```cpp
void run()
```


### function close

```cpp
void close()
```


### function broadcastUdp

```cpp
int broadcastUdp(
    const cmn::packetData & packet,
    uint16_t port
)
```


### function broadcastTcp

```cpp
int broadcastTcp(
    const cmn::packetData & packet
) const
```


### function bindPorts

```cpp
int bindPorts(
    uint16_t port
)
```


### function Server

```cpp
explicit Server(
    const std::shared_ptr< cmn::SharedData > & data
)
```


-------------------------------

Updated on 2025-12-13 at 17:56:13 +0000
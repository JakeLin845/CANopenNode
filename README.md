# CANopen Node Library for Arduino

This project is a **lightweight CANopen slave stack for Arduino**, built as part of my exploration into the CANopen protocol for a separate project, and later expanded into a more complete library. 

It enables an **Arduino Uno with MCP2515 CAN controller + Seeed CAN-BUS
Shield** to participate in a CANopen network as a **slave device**.

Supported features include:
- **NMT (Slave only)**
- **PDOs** (Process Data Objects, configurable mapping)
- **SDOs** (Service Data Objects for configuration & diagnostics)
- **Heartbeat** producer
- **EMCY** (Emergency messaging)
- **Object Dictionary** for node variables

Development was done with the [Seeed Studio CAN-BUS Shield V1.2](https://wiki.seeedstudio.com/CAN-BUS_Shield_V1.2/) using an MCP2515 CAN Controller on an Arduino Uno. 

**Testing and verification** were completed with a PCAN device acting as a simulated master and in real-world setups against a master node.

---

## Table of Contents

- [Setup](#setup)
- [Usage](#usage)
- [Functions](#functions)
- [Object Specific Functions](#object-dictionary-functions)
- [Dependencies](#dependencies)
- [License](#license)
- [References](#references--resources)
- [Contributing](#contributing)

---

## Quick Start

See the examples folder for full sketches and more detailed examples of usage. Make sure you adjust the object dictionary and config files for your application.

```cpp
#include "canOpenNode.h"

// Example: Use SPI CS pin 9 and INT pin 2, 250 kbps
CanOpenNode node(9, 2, CAN_250KBPS);

void setup() {
  Serial.begin(115200);
  node.begin(); // initialize MCP2515 and CANopen stack
}

void loop() {
  node.run(); // process incoming/outgoing CANopen messages
}
```

---

## Setup

Interact with and adjust the following to match your application: 

- `CanOpenNode(uint8_t csPin, uint8_t intPin, uint8_t baudRate)` — constructor
- `void begin()` — initialize the MCP2515 and CANopen stack
- `void run()` — main processing loop
- `config.h` — configuration (IDs, PDO/heartbeat timings)
- `objDict.cpp` — object dictionary (application variables)
- `errTable.cpp` — EMCY message definitions

---

## Functions

| Function | Description |
|---|---|
| `bool isActive()` | Return true when the CAN bus is active |
| `void disableNMT()` | Disable NMT state handling |
| `void disableMapping()` | Disable PDO mapping |
| `bool sendMsg(const Message &msg)` | Send a CAN message |
| `void writeData(Object &obj, uint8_t data[4])` | Write 4 bytes into an Object Dictionary entry |
| `void writeData(Object &obj, uint8_t data[8], uint8_t start)` | Write bytes into an Object starting at `start` (for RX PDOs) |
| `NMT getNMT()` | Get current NMT |
| `PDO* getPDO(PDO::Type type, uint8_t num)` | Get pointer to the specified PDO |
| `SDO getSDO()` | Get current SDO |
| `Receiver getReceiver()` | Get the most recently received message (Receiver) |
| `EMCY getEmcy()` | Get current EMCY message |
| `void setEmcyErr(Error err)` | Set EMCY using an Error enum/value |
| `void setEmcyErr(const uint16_t code)` | Set EMCY using a numeric error code |
| `uint16_t getEmcyCode()` | Return the current EMCY error code |

---

## Object Dictionary Functions

| Function | Description |
|---|---|
| `static int32_t findIndex(uint16_t index, uint8_t subIndex)` | Returns the dictionary array index for the specified object (returns `-1` if not found). |
| `uint8_t getTypeSize()` | Returns the size in bytes of the object's data type. |
| `void printInfo()` | Prints detailed object information to Serial output (useful for debugging). |

---

## Dependencies

This project makes use of [Cory J. Fowler's MCP CAN Library](https://github.com/coryjfowler/MCP_CAN_lib) for interfacing with the MCP2515 CAN controller. A big thanks to Cory and others for developing and maintaining this essential library that simplifies CAN communication on Arduino platforms.

---

## License

This project is licensed under the [MIT License](LICENSE).

---

## References & Resources

- [CANopen Protocol Overview](https://www.can-cia.org/can-knowledge/canopen/)
- [Seeed Studio CAN-BUS Shield V1.2](https://wiki.seeedstudio.com/CAN-BUS_Shield_V1.2/)
- [MCP2515 CAN Controller Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/MCP2515-Stand-Alone-CAN-Controller-with-SPI-20001801J.pdf)

---

## Contributing

Feel free to contribute, raise issues, or submit feature requests! This project is still in an early stage and could definitely see improvements in various areas. 

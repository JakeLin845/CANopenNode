# CANopen Node Library for Arduino

A CANopen Node simulator developed for Arduino supporting core protocols such as SDOs, PDOs, PDO mapping, NMT modes, NMT heartbeat, and EMCY message selection.

Currently, this library acts **only as a slave device** and does not support NMT master functionality.

Development was done with the [Seed Studio CAN-BUS Shield V1.2](https://wiki.seeedstudio.com/CAN-BUS_Shield_V1.2/) using an MCP2515 CAN Controller on an Arduino Uno.

---

## Table of Contents

- [Setup](#setup)
- [Usage](#usage)
- [Functions](#functions)
- [Object Specific Functions](#object-specific-functions)
- [License](#license)

---

## Setup

To get started, the user will interact with the following components:

- `CanOpenNode(uint8_t csPin, uint8_t intPin, uint8_t baudRate)` — Object constructor
- `void begin()` — Initializes the MCP2515 controller and sets up the node
- `void run()` — Runs the core communication loop
- `config.h` — Configure parameters such as message IDs, cycle times for PDOs and heartbeat
- `objDict.cpp` — The object dictionary containing indices and data fields to customize
- `errTable.cpp` — Table of EMCY messages; edit to change message contents

---

## Usage

    #include "main.h"

    // Define the pins for your CAN-BUS shield
    // Default for Seeed Studio Shield is CS: 9, INT: 2
    constexpr uint8_t CS_PIN = 9;
    constexpr uint8_t INT_PIN = 2;

    // Create an instance of your CANopen node
    CanOpenNode node(CS_PIN, INT_PIN, CAN_250KBPS);

    void setup() {
    Serial.begin(115200);
    
    Serial.println("CANopen Node starting up...");

    // Initialize the CANopen node and set up the MCP2515 CAN controller
    node.begin();

    Serial.println("Setup complete. Running node...");
    
    // Enable/Disable NMT state machine and PDO mapping (Default is enabled)
    // node.disableNMT(true); 
    // node.disableMapping(true);

    // Manually set PDOs here if mapping isn't used
    // node.set(PDO::Type::TX, 1, STATUS, 0x01, 0);

    // node.set(PDO::Type::TX, 2, TEMPERATURE, 0x00, 2);

    // node.set(PDO::Type::RX, 1, VOLTAGE, 0x05, 4);
    }

    void loop() {
    // Continuously run the CANopen node to process incoming and outgoing messages
    node.run();
    }

---

## Functions

| Function                             | Description                                 |
|------------------------------------|---------------------------------------------|
| `bool isActive()`                   | Returns `true` when the CAN bus is active   |
| `void disableNMT()`                 | Disables NMT state handling                  |
| `void disableMapping()`             | Disables PDO mapping                          |
| `bool sendMsg(const Message &msg)`         | Sends a message object to the CAN bus       |
| `void writeData(Object &obj, uint8_t data[4])` | Writes specified data into object dictionary entry |
| `NMT getNMT()`                     | Returns the current NMT message               |
| `PDO* getPDO(PDO::Type type, uint8_t num)`          | Returns the specified PDO message            |
| `SDO getSDO()`                     | Returns the current SDO message               |
| `Receiver getReceiver()`            | Returns the current incoming message          |
| `EMCY getEmcy()`                   | Returns the current selected EMCY message     |
| `void setEmcyErr(Error err), void setEmcyErr(const uint16_t code)`| Sets the EMCY message to the specified error |
| `uint16_t getEmcyCode()`            | Returns the EMCY error code                    |

---

## Object Specific Functions

- `static int32_t findIndex(uint16_t index, uint8_t subIndex)` — Returns the dictionary array index for the specified object
- `uint8_t getTypeSize()` — Returns the size (in bytes) of the object's data type
- `void printInfo()` — Prints detailed object information to Serial output

---

## Acknowledgements

This project makes use of [Cory J. Fowler's MCP CAN Library](https://github.com/coryjfowler/MCP_CAN_lib) for interfacing with the MCP2515 CAN controller. A big thanks to Cory for developing and maintaining this essential library that simplifies CAN communication on Arduino platforms.

---

## License

This project is licensed under the [MIT License](LICENSE).

---

## References & Resources

- [CANopen Protocol Overview](https://www.can-cia.org/can-knowledge/canopen/)
- [Seed Studio CAN-BUS Shield V1.2](https://wiki.seeedstudio.com/CAN-BUS_Shield_V1.2/)
- [MCP2515 CAN Controller Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/MCP2515-Stand-Alone-CAN-Controller-with-SPI-20001801J.pdf)

---

*Feel free to contribute or raise issues!*

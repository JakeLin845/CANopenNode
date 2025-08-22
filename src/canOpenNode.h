#ifndef CAN_OPEN_NODE_H
#define CAN_OPEN_NODE_H

#include <mcp_can.h> // CoryJFowler MCP2515 library (https://github.com/coryjfowler/MCP_CAN_lib)
#include "SPI.h"

#include "config.h"
#include "message.hpp"
#include "receiver.hpp"
#include "pdo.hpp"
#include "sdo.hpp"
#include "nmt.hpp"
#include "emcy.hpp"
#include "object.hpp"

class CanOpenNode {
public:
    CanOpenNode(uint8_t csPin, uint8_t intPin, uint8_t baudRate);

    // Initialize the MCP2515 CAN controller and sets up the node
    void begin();
    // Run core communication loop
    void run();
    // Manually set PDO mapping
    void set(PDO::Type type, uint8_t num, uint16_t index, uint8_t subIndex, uint8_t position);

    // Public - Core communication functions
    bool sendMsg(const Message &msg);
    void writeData(Object &obj, uint8_t data[4]);
    // EMCY - Getters/Setters
    EMCY getEmcy() const {return emcy;};
    void setEmcyErr(const Error &err) {emcy.setError(err);};
    void setEmcyErr(const uint16_t code) {emcy.setError(code);};
    uint16_t getEmcyCode() const {return emcy.getErrCode();};
    // Receiver - Getters/Setters
    Receiver getReceiver() const {return recv;};
    bool isActive() const {return recv.active;};
    // NMT - Getters/Setters
    NMT getNMT() const {return nmt;};
    // PDO - Getters/Setters
    PDO* getPDO(PDO::Type type, uint8_t num) const {
        if (num < 1 || num > 4) return nullptr;
        return (type == PDO::Type::TX) ? &txPdo[num - 1] : &rxPdo[num - 1];
    };
    // SDO - Getters/Setters
    SDO getSDO() const {return sdo;};
    // Enable/Disable Boolean flags (Default is enabled)
    void disableNMT(bool disable) { nmtOn = !disable; };
    void disableMapping(bool disable) { mappingOn = !disable; };
private:
    MCP_CAN can;
    const uint8_t intPin;
    const uint8_t baudRate;
    volatile bool flagRecv;
    // Boolean flags 
    bool nmtOn; // NMT handling enabled
    bool mappingOn; // PDO mapping enabled
    // Current NMT state
    NMT::Mode nmtState;
    // CANopen message objects
    Receiver recv;
    SDO sdo;
    NMT nmt;
    EMCY emcy;
    PDO txPdo[4];
    PDO rxPdo[4];   
    // Private - Core communication functions
    // NMT
    void nmtController();
    void heartBeat();
    // PDO
    void mapPDOArray(PDO* pdoArray, uint16_t baseIndex);
    void mapPDOs();
    void sendPDO(PDO &txPdo);
    void receivePDO(PDO &rxPdo);
    void writeData(Object &obj, uint8_t data[8], uint8_t start);
    // SDO
    void sdoHandler();
    // ISR Handling 
    static CanOpenNode* instance;
    void MCP2515_ISR();
    static void ISRhandler();
};

#endif
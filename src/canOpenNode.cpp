#include "CanOpenNode.h"
#include <Arduino.h>

// Initialize static instance pointer for ISR
CanOpenNode* CanOpenNode::instance = nullptr;

CanOpenNode::CanOpenNode(uint8_t csPin, uint8_t intPin, uint8_t baudRate) : 
    can(csPin),
    intPin(intPin),
    baudRate(baudRate),
    flagRecv(false),
    nmtState(NMT::Mode::BOOT), 
    recv(&can),
    sdo(SDO_TX_ID), 
    nmt(NMT_ID, HEARTBEAT_CYCLE_TIME),
    emcy(EMCY_ID),
    txPdo{PDO(PDO1_TX_ID, PDO::Type::TX, PDO1_TX_CYCLE_TIME), 
          PDO(PDO2_TX_ID, PDO::Type::TX, PDO2_TX_CYCLE_TIME),
          PDO(PDO3_TX_ID, PDO::Type::TX, PDO3_TX_CYCLE_TIME),
          PDO(PDO4_TX_ID, PDO::Type::TX, PDO4_TX_CYCLE_TIME)},
    rxPdo{PDO(PDO1_RX_ID, PDO::Type::RX, PDO1_RX_CYCLE_TIME),
          PDO(PDO2_RX_ID, PDO::Type::RX, PDO2_RX_CYCLE_TIME),
          PDO(PDO3_RX_ID, PDO::Type::RX, PDO3_RX_CYCLE_TIME),
          PDO(PDO4_RX_ID, PDO::Type::RX, PDO4_RX_CYCLE_TIME)}, 
    nmtOn(true), 
    mappingOn(true)
{
    instance = this; 
}

// Begin function to initialize the CAN bus and set up the node
void CanOpenNode::begin() 
{
    if (can.begin(MCP_ANY, baudRate, MCP_16MHZ) != CAN_OK) 
    {
        while(1) 
        {
            Serial.println("CAN Init Failed. Halting.");
            delay(250);
        }
    }

    Serial.println("MCP2515 Initialized Successfully!");
    can.setMode(MCP_NORMAL);
    pinMode(intPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(intPin), ISRhandler, FALLING);
}

// Run function to handle the core communication loop with NMT handling
void CanOpenNode::run() {
    recv.read(&flagRecv);
    if (nmtOn)
    {
        nmtController(); 

        switch(nmtState) 
        {
            case NMT::Mode::BOOT:
                sendMsg(nmt);
                nmtState = NMT::Mode::PRE_OPERATIONAL;
                break;

            case NMT::Mode::PRE_OPERATIONAL:
                if (mappingOn) mapPDOs();
                sdoHandler();
                break;

            case NMT::Mode::OPERATIONAL:
                heartBeat();
                for(int i = 0; i < 4; ++i) 
                {
                    sendPDO(txPdo[i]);
                    receivePDO(rxPdo[i]);
                }
                sdoHandler();
                break;

            case NMT::Mode::STOPPED:
                break;
        }
    }
    else 
    {
        if (mappingOn) mapPDOs();
        nmt.changeMode(NMT::Mode::OPERATIONAL);
        heartBeat();
        for(int i = 0; i < 4; ++i) 
        {
            sendPDO(txPdo[i]);
            receivePDO(rxPdo[i]);
        }
        sdoHandler();
    }
}

// Manually set PDO mapping
void CanOpenNode::set(PDO::Type type, uint8_t num, uint16_t index, uint8_t subIndex, uint8_t position) {
    PDO* pdo;
    uint8_t pdoNum = num - 1; 
    if (pdoNum > 3) return; // PDO num is not between 1 to 4

    if (type == PDO::Type::RX) pdo = &(rxPdo[pdoNum]);
    else if (type == PDO::Type::TX) pdo = &(txPdo[pdoNum]); 

    pdo->set(index, subIndex, position);
    pdo->numObjects = 0; 
    // update numObjects count
    for (int i = 0; i < 8; i++) 
    {
        if (pdo->getObject(i) != nullptr) pdo->numObjects++;
    }
}

// Send a CAN message
bool CanOpenNode::sendMsg(const Message &msg) {
    if (can.sendMsgBuf(msg.id, 0, msg.dlc, msg.data) != CAN_OK) 
    {
        return false;
    }
    return true;
}

// Write data to an object in the Object Dictionary
void CanOpenNode::writeData(Object &obj, uint8_t data[4]) {
    if (obj.access == ObjectPermissions::READ) return;
    memcpy(obj.data, data, obj.getTypeSize());
}
// Write data to an object in the Object Dictionary with a specified start position (ex. rxPDOs)
void CanOpenNode::writeData(Object &obj, uint8_t data[8], uint8_t start) {
    // if (obj.access == ObjectPermissions::READ) return;
    uint8_t size = obj.getTypeSize();
    if (start + size > 8) return;
    memcpy(obj.data, &data[start], size);
}

// Controls NMT state transitions
void CanOpenNode::nmtController() {
    if (recv.id == 0x00) 
    { 
        uint8_t command = recv.data[0];
        uint8_t nodeId = recv.data[1];

        if (nodeId == NMT_ID || nodeId == 0x00) 
        {
            switch(command) 
            {
                case 0x01: 
                    nmtState = NMT::Mode::OPERATIONAL; break;
                case 0x02: 
                    nmtState = NMT::Mode::STOPPED; break;
                case 0x80: 
                    nmtState = NMT::Mode::PRE_OPERATIONAL; break;
                case 0x81: 
                case 0x82: 
                    nmtState = NMT::Mode::BOOT; break;
            }
            nmt.changeMode(nmtState);
        }
        recv.clearMsg();
    }
}

// Send an NMT heartbeat message at regular intervals
void CanOpenNode::heartBeat() {
    if ((millis() - nmt.timer) > nmt.cycleTime) 
    {
        sendMsg(nmt);
        nmt.timer = millis();
    }
}

// Map PDOs from the Object Dictionary to the PDO structure
void CanOpenNode::mapPDOArray(PDO pdoArray[4], uint16_t baseIndex) {
    for (int i = 0; i < 4; ++i) 
    {
        PDO* pdo = &pdoArray[i];
        if (pdo->id == 0xFFF) continue; // Skip unconfigured PDOs

        uint16_t baseMapIndex = baseIndex + i;

        int mapCountIndex = Object::findIndex(baseMapIndex, 0x00);
        if (mapCountIndex < 0) continue;

        uint8_t numMappedObjects = dictionary[mapCountIndex].data[0];
        pdo->numObjects = numMappedObjects;
        if (numMappedObjects == 0 || numMappedObjects > 8) continue;

        uint8_t currentBytePosition = 0;
        for (uint8_t sub = 1; sub <= numMappedObjects; ++sub) 
        {
            if (currentBytePosition >= PDO_LEN) break;

            int entryIndex = Object::findIndex(baseMapIndex, sub);
            if (entryIndex < 0) continue;

            Object& entry = dictionary[entryIndex];
            uint8_t bitLength  = entry.data[0]; // in bits 
            uint8_t subIndex   = entry.data[1];
            uint16_t objIndex  = (entry.data[3] << 8) | entry.data[2];

            uint8_t byteLength = bitLength / 8;

            if (currentBytePosition + byteLength > PDO_LEN) break;

            if (pdo->set(objIndex, subIndex, currentBytePosition)) 
            {
                currentBytePosition += byteLength;
            } 
            else 
            {
                #ifdef DEBUG
                Serial.print("Failed to map object to PDO"); Serial.println(i + 1);
                #endif
                break;
            }
        }
    }
}
// Map Tx and Rx PDOs
void CanOpenNode::mapPDOs() {
    // Map Transmit PDOs (TPDOs)
    mapPDOArray(txPdo, 0x1A00);

    // Map Receive PDOs (RPDOs)
    mapPDOArray(rxPdo, 0x1600);
}

// Send txPDOs at specified cycle times
void CanOpenNode::sendPDO(PDO &txPdo) {
    if ((millis() - txPdo.timer) > txPdo.cycleTime) 
    {   
        if (txPdo.numObjects == 0) return; // skip sending empty PDO
        // PDO type safeguard
        if (txPdo.type != PDO::Type::TX) return;
    
        txPdo.updateData();
        sendMsg(txPdo);
        txPdo.timer = millis();
    }
}

// Receive rxPDOs and write data to the Object Dictionary
void CanOpenNode::receivePDO(PDO &rxPdo) {
    if (recv.id != rxPdo.id) return;

    if (rxPdo.type != PDO::Type::RX) return;

    for (int i = 0; i < 8; i++) 
    {
        if (rxPdo.getObject(i) != nullptr) 
        {
            writeData(*(rxPdo.getObject(i)), recv.data, i);
        }
    }
    recv.clearMsg();
}

// Updates SDO fields accordingly for requests and responses 
void CanOpenNode::sdoHandler() {
    // sets response if message is a valid RX SDO
    if (sdo.setResponse(recv)) 
    {
        if (sdo.sendType == SDO::SendType::WRITE) 
        { // write message to object dictionary
            uint8_t tempData[4] = {recv.data[SDO::DATA_4], recv.data[SDO::DATA_5],
                                   recv.data[SDO::DATA_6], recv.data[SDO::DATA_7]};
            Object* obj = sdo.getObject();
            if (obj != nullptr) 
                writeData(*obj, tempData);
        }
        if (sendMsg(sdo)) // send and clear message
            recv.clearMsg();
    }
}

// Interrupt Service Routine logic
void CanOpenNode::MCP2515_ISR() {
    flagRecv = true;
}
static void CanOpenNode::ISRhandler() {
    if (instance != nullptr) 
    {
        instance->MCP2515_ISR();
    }
}
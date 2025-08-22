#ifndef SDO_H
#define SDO_H

#include "message.hpp"

struct Receiver; // forward declaration
 
// MESSAGE FORMAT: | Command Byte | Index 0 | Index 1 | Sub Index | Data (4b) |
struct SDO : public Message {
    public:
    enum class SendType : uint8_t {
        WRITE,
        READ, 
        INVALID
    }; // from the perspective of Slave to Master (write to, read from)
    SendType sendType;
    // Name of each byte in the SDO message
    enum DataField : uint8_t {
        COMMAND = 0,
        INDEX_LOW = 1,
        INDEX_HIGH = 2,
        SUB_INDEX = 3,
        DATA_4 = 4,
        DATA_5 = 5,
        DATA_6 = 6,
        DATA_7 = 7
    };

    SDO(uint16_t id) : Message(id, SDO_LEN), obj(nullptr), sendType(SendType::INVALID) {};

    const Object *getObject() const { return obj; };
    
    bool setResponse(const Receiver &recv); // Smartly set the SDO object data fields based on the received message

    private:
    Object *obj; // store object this SDO is referencing

    // Set object this SDO is referencing 
    void setObject(int32_t index) {
        if (index >= 0)
            obj = &dictionary[index];
        else
            obj = nullptr;
    };
    // Pass in object's datatype as the index to access relevant command byte
    inline static constexpr uint8_t readCommandByte[5] = {
        0,
        READ_REQ_1BYTE_CMD,
        READ_REQ_2BYTE_CMD,
        0,
        READ_REQ_4BYTE_CMD
    };
    
    uint8_t getCmd() { // Return the appropriate command byte for the current object
     // Send type must already be set before calling this function
        if (sendType == SendType::WRITE)
            return WRITE_RESP_SUCCESS_CMD; 
        else if (sendType == SendType::READ)
            return readCommandByte[static_cast<uint8_t>(obj->type)];
        else 
            return 0xFF;
    };    
};

#endif
#ifndef EMCY_H
#define EMCY_H

#include "message.hpp"

struct Error {
    const uint16_t errorCode; 
    const uint8_t errorRegister; 
    const uint8_t specificError[5];
};

extern const Error errorTable[]; // Table of predefined selection of EMCY errors
extern const uint8_t NUM_EMCYS;

class EMCY : public Message {
    public: 
    // Name of each byte in the EMCY message
    enum DataField : uint8_t {
        ERR_CODE_LOW   = 0,
        ERR_CODE_HIGH  = 1,
        ERR_REG        = 2,
        SPECIFIC_ERR_3 = 3,
        SPECIFIC_ERR_4 = 4,
        SPECIFIC_ERR_5 = 5,
        SPECIFIC_ERR_6 = 6,
        SPECIFIC_ERR_7 = 7
    };

    EMCY(uint16_t id) : 
        Message(id, EMCY_LEN), 
        error(&(errorTable[0])) 
    { 
        updateEmcy(); 
    }

    // Set the current error held by this EMCY message object
    void setError(const Error &err) {
        error = &err;
        updateEmcy();
    }
    void setError(uint16_t code) {
        for (uint8_t i = 0; i < NUM_EMCYS; i++) 
        {
            if (errorTable[i].errorCode == code) 
            {
                error = &errorTable[i];
                updateEmcy();
                return;
            }
        }
        // If not found, default to the first error
        error = &errorTable[0];
        updateEmcy();
    }

    // Getters
    uint16_t getErrCode() { return (data[ERR_CODE_LOW] | (data[ERR_CODE_HIGH] << 8)); }

    private: 
    Error *error;

    // Update EMCY message object contents to match the current error from the table
    void updateEmcy() {
        data[ERR_CODE_LOW] = error->errorCode & 0xFF;
        data[ERR_CODE_HIGH] = error->errorCode >> 8;
        data[ERR_REG] = error->errorRegister;
        memcpy(&data[SPECIFIC_ERR_3], error->specificError, 5);
    }
};

#endif
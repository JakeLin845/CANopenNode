#ifndef NMT_H
#define NMT_H

#include "message.hpp"

class NMT : public Message {
    public:
    // Internal timer for NMT transmission
    uint32_t timer; 
    uint16_t cycleTime;

    // Predefined NMT modes
    enum class Mode : uint8_t {
        BOOT = 0X00,
        PRE_OPERATIONAL = 0X7F,
        OPERATIONAL = 0X05,
        STOPPED = 0X04
    };

    NMT(uint16_t id, uint16_t cycleTime) : 
        Message(id, NMT_LEN), 
        cycleTime(cycleTime), 
        timer(0) 
    {
        mode = Mode::BOOT; 
        data[0] = static_cast<uint8_t>(mode); // set initial mode to BOOT
    };

    // Change the NMT mode
    void changeMode(Mode mode) {
        this->mode = mode;
        data[0] = static_cast<uint8_t>(this->mode); 
    }

    // Getters
    uint8_t getMode() const {return static_cast<uint8_t>(mode);}

    private: 
    Mode mode;
};
#endif
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

#define DEBOUNCE_TIME 30

// Buttons
struct Button
{
    Button(uint8_t buttonPin) : pin(buttonPin) {};

    const uint8_t pin; 
    bool input;
    bool lastInput; 
    bool isPressed; 
    bool output;
    uint32_t timer = 0; 

    // Prototypes
    bool debounce(); 
};

#endif 
#include "button.hpp"

// button debouncer
bool Button::debounce()
{
    uint32_t currentTime = millis(); 
    input = !digitalRead(pin); // active low button
    output = 0; 
    
    if (input != lastInput) // reset debounce if input changes
    {
        timer = currentTime; 
    }
    // output high only once per button press (comment section out if unwanted)
    if (input == 1 && (currentTime - timer > DEBOUNCE_TIME) && !isPressed) 
    {
        isPressed = 1; 
        output = 1; 
    }
    else if(input == 0) 
    { 
        isPressed = 0; 
    }

    lastInput = input; 

    return output; 
}

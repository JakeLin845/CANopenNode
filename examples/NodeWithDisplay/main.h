#ifndef MAIN_H
#define MAIN_H

#include "TFT_ILI9341.h" // TFT_ILI9341 by Bodmer https://github.com/Bodmer/TFT_ILI9341/tree/master/examples
#include "canOpenNode.h"
#include "button.hpp"

// Inputs
#define BUTN1     3
#define BUTN2     4 
#define POT       A5
// CAN Open Node object 
extern CanOpenNode node;
// TFT display object
extern TFT_ILI9341 tft; 
// Button objects
extern Button red; 
extern Button white; 

// Main state machine
enum class MainState : uint8_t
{
  SETUP, // wait for CAN communication
  RUN // run periodic procedures
};
extern MainState state;
// Display controller state machine
enum class TftState : uint8_t
{
  SETUP,
  SETUP_SENDING,
  SETUP_EXIT,
  UPDATE_EMCY,
  SEND_CONFIRM, 
  SEND_DECLINE, 
  SEND_SUCCESS
};
extern TftState scene; 

// Returns true if specified millis have passed since timer value
inline bool hasElapsed(uint16_t ms, uint32_t timer) {return (millis() - timer) > ms;}

void EMCY_Controller();
void Charge_Controller();

void displayController(); // controls the current scene shown on the TFT display
void TFT_SETUP(); // splash screen
void TFT_UPDATE_SETUP(); // update splash screen when sending SDOs
void TFT_EXIT_SETUP(); // exit splash screen on successful startup
void TFT_EMCY(); // display selected EMCY error code
void TFT_DSOC(); // display charge
void TFT_SEND_CONFIRM(); // display confirmation screen for sending EMCY
void TFT_DECLINE_CONFIRM(); // clear confirmation screen for sending EMCY
void TFT_SEND_SUCCESS(); // display success message after sending EMCY

#endif
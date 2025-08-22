#include "main.h"
/* This example demonstrates a basic CANopen node setup using the MCP2515 CAN controller 
integrated into a larger project involving a TFT display and various inputs. */

// **IMPORTANT** NOTE: objDict.cpp and config.h should only be linked in the main project, not in this example. Please ensure only one is linked at a time to avoid conflicts.

// Define the pins for your CAN-BUS shield.
// Default for Seeed Studio Shield is CS: 9, INT: 2
constexpr uint8_t CS_PIN = 9;
constexpr uint8_t INT_PIN = 2;

// Create an instance of your CANopen node.
CanOpenNode node(CS_PIN, INT_PIN, CAN_250KBPS); // use baud rate macros

// Define the TFT display object.
TFT_ILI9341 tft;

// Button objects
Button red(BUTN2);
Button white(BUTN1);

// State enum
MainState state = MainState::SETUP; // initial state

// Display enum
TftState scene = TftState::SETUP;

void setup() {
  Serial.begin(115200);
  
  pinMode(BUTN1, INPUT_PULLUP);
  pinMode(BUTN2, INPUT_PULLUP);
  pinMode(POT, INPUT_PULLUP);

  Serial.println("CANopen Node starting up...");

  // Initialize the CANopen node. This will handle CAN controller setup.
  node.begin();

  Serial.println("Setup complete. Running node...");
  
  // Enable/Disable NMT state machine and PDO mapping (Default is enabled)
  node.disableNMT(true); 
  node.disableMapping(true);
  
  // Manually set PDOs here if mapping isn't used
  node.set(PDO::Type::TX, 1, STATUS, 0x01, 0);
  node.set(PDO::Type::TX, 1, DISPLAYED_CHARGE, 0x01, 1);
  node.set(PDO::Type::TX, 1, VOLTAGE, 0x01, 4);
  node.set(PDO::Type::TX, 1, CURRENT, 0x01, 6);

  node.set(PDO::Type::TX, 2, TEMPERATURE, 0x01, 4);

  node.set(PDO::Type::RX, 1, MIN_VOLTAGE, 0x03, 1);
}

void loop() {
  displayController();
  node.run();
  switch (state)
  {
    case MainState::SETUP:
      if (node.isActive()) // go to next state if CAN bus is active
      {
        scene = TftState::SETUP_SENDING;

        static uint32_t timer = millis(); 
        if (hasElapsed(10, timer))
        {
          state = MainState::RUN;
          scene = TftState::SETUP_EXIT;
        }
      }
      break;
    case MainState::RUN:
      EMCY_Controller();
      Charge_Controller();
      break;
    default: 
      Serial.println("Default State Error");
      break;
  };
}
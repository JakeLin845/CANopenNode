#include "canOpenNode.h"
// This example demonstrates a basic CANopen node setup using the MCP2515 CAN controller.

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
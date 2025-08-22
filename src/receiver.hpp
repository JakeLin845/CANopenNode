#ifndef RECEIVER_H
#define RECEIVER_H

#include "message.hpp"

// Receiver message type
struct Receiver : public Message {
  MCP_CAN *CAN;
  bool active;
  // specify CAN bus for receiver message
  Receiver(MCP_CAN *CAN) : 
    CAN(CAN), 
    active(false)
  {};

  void read(bool *flagRecv); // read message from CAN bus
  void printBuf(); // print message to serial monitor
};

#endif
#ifndef MESSAGE_H
#define MESSAGE_H

#include <mcp_can.h>
#include "object.hpp"

// Generic message type
class Message {
  public:
  uint32_t id;  
  uint8_t  dlc; // data length code

  uint8_t data[8];
  
  Message() : 
    id(0), 
    dlc(0) 
  {
    memset(data, 0, sizeof(data));
  }; 
  
  Message(uint32_t id, uint8_t dlc) : 
    id(id), 
    dlc(dlc) 
  {
    memset(data, 0, sizeof(data));
  };

  void clearMsg();
  void clearData();

  virtual ~Message() {}; 
};

#endif

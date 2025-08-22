#ifndef PDO_H
#define PDO_H

#include "message.hpp"
class PDO : public Message {
  public:
  uint8_t numObjects;  // number of objects mapped to PDO message data field
  // Internal timer for PDO transmission
  uint32_t timer; 
  uint16_t cycleTime;
  // PDO type: RX for Receive PDO, TX for Transmit PDO
  enum class Type {
    RX, 
    TX
  };
  const Type type;

  PDO(uint16_t id, Type type, uint16_t cycleTime) : 
    Message(id, PDO_LEN), 
    type(type), 
    cycleTime(cycleTime), 
    timer(0), 
    numObjects(0)
  {
    for (int i = 0; i < 8; i++)
    {
      objList[i] = nullptr; // initialize object list to nullptr
    } // unused bytes and non-starting bytes will be nullptr values
  };

  bool set(Object &obj, uint8_t position); // Map object to PDO object list at specified byte position
  bool set(uint16_t index, uint8_t subIndex, uint8_t position); // Map object at index and subindex to PDO object list at specified byte position
  void updateData(); // Update PDO data buffer to match object dictionary values

  // Getters
  Object *getObject(uint8_t position) const {
    if (position < 8)
      return objList[position];
    return nullptr; // return nullptr if position is out of bounds
  };

  private: 
  Object *objList[8]; // list of objects mapped to PDO message data field
};

#endif
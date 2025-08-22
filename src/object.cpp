#include "object.hpp"
#include <Arduino.h>

// Return object index position in dictionary array
int32_t Object::findIndex(uint16_t index, uint8_t subIndex) {
  // loop through object dictionary to find matching indices
  for (int i = 0; i < NUM_OBJS; ++i)
  {
    if (index == dictionary[i].index && subIndex == dictionary[i].subIndex)
    {
      return i;
    }
  }
  return -1; // return -1 if no match found
}

// Print read message buffer to serial
void Object::printInfo() {
  char msgString[128] = {0}; 

  snprintf(msgString, sizeof(msgString), "Object Dictionary - Index: 0x%.3X  Subindex: %1X  Data:", index, subIndex);

  Serial.print(msgString);

  // Print data[] contents
  for (uint8_t i = 0; i < this->getTypeSize(); i++)
  {
    snprintf(msgString, sizeof(msgString), " 0x%.2X", data[i]);
    Serial.print(msgString);
  }

  Serial.println();
}
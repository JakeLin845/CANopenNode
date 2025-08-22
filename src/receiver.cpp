#include "receiver.hpp"

// Update read message buffer
void Receiver::read(bool *flagRecv) {
  if (*flagRecv) // if CAN_INT pin is low, read receive buffer
  {
    *flagRecv = 0;
    active = true; // set receiver active

    while (CAN_MSGAVAIL == CAN->checkReceive())
    {
      CAN->readMsgBuf(&id, &dlc, data);
      #ifdef PRINT
        printBuf(); 
      #endif
    }
  }
  else active = false; 
}

// Print read message buffer to serial
void Receiver::printBuf() {
  char msgString[128] = {0}; 

  snprintf(msgString, sizeof(msgString), "Standard ID: 0x%.3lX       DLC: %1d  Data:", id, dlc);

  Serial.print(msgString);

  // Print buffer contents
  for (uint8_t i = 0; i <dlc; i++)
  {
    snprintf(msgString, sizeof(msgString), " 0x%.2X", data[i]);
    Serial.print(msgString);
  }

  Serial.println();
}
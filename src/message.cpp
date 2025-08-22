#include "message.hpp"

// Clear all message fields
void Message::clearMsg() {
  id = 0; 
  dlc = 0; 
  clearData();
}

// Clear message data
void Message::clearData() {
  memset(data, 0, sizeof(data));
}
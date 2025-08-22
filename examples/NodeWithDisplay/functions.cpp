#include "main.h"

// Controller for sending EMCY messages
void EMCY_Controller()
{
  static bool sent = 0; // sent flag 
  static Error* error = &errorTable[0]; // current error 

  enum state : uint8_t
  {
    SELECT, 
    CONFIRM, 
    SEND 
  }; static state sendState = SELECT; // initial state

  if (white.debounce() && sendState < SEND) 
    sendState = sendState + 1; 
  switch (sendState) 
  {
    case SELECT: // choose EMCY to send
      scene = TftState::UPDATE_EMCY;
      if (red.debounce()) // cycle through EMCY codes
      {
        if (error < &errorTable[NUM_EMCYS - 1])
          error++;
        else
          error = &errorTable[0];
        node.setEmcyErr(*error); // iterate to next EMCY message
        #ifdef DEBUG
        Serial.print("Selected Error Code: ");
        Serial.println(EMCY_Table[*index].error_code, HEX);
        #endif
      }
      break;
    case CONFIRM: // confirm EMCY to send
      scene = TftState::SEND_CONFIRM;
      if (red.debounce()) // cancel send 
      {
        sendState = SELECT; 
        scene = TftState::SEND_DECLINE;
      }
      break;
    case SEND: // send chosen EMCY 
      if (!sent) 
      {
        if (node.sendMsg(node.getEmcy())) 
        {
          sent = 1;
          #ifdef DEBUG
          Serial.print("EMCY Specific Error: "); 
          Serial.print(EMCY_Table[*index].specific_error[3], HEX);
          Serial.println(EMCY_Table[*index].specific_error[4], HEX);
          #endif 
          scene = TftState::SEND_SUCCESS;
        }
      }
      break;
  }
}

// Controller for changing displayed charge
void Charge_Controller()
{
  static uint32_t timer = millis(); 
  static const uint32_t index = Object::findIndex(DISPLAYED_CHARGE, 0x01); 
  uint8_t charge = 0;

  if (hasElapsed(100, timer)) // sample displayed charge every 100 ms
  {
    charge = map(analogRead(POT), 25, 1020, 1, 100); 
    dictionary[index].data[0] = charge; // update displayed charge
    timer = millis(); 
  }
}

#include "main.h"

// controls the current scene shown on the TFT display
void displayController()
{
  static int8_t lastScene = -1; // -1 indicates no scene has been set yet
  static uint32_t timer = millis(); 
  // when scene is SETUP_EXIT, execute immediately, don't wait for timer
  if (hasElapsed(125, timer) || scene == TftState::SETUP_EXIT) // poll for display updates every 125ms (8 fps)
  {  // update scene only if it has changed, or the EMCY index has changed
    if (lastScene != static_cast<uint8_t>(scene) || scene == TftState::UPDATE_EMCY) // continually update scene in UPDATE_EMCY
    {
      switch (scene)
      {
        case TftState::SETUP: 
          TFT_SETUP();
          break;
        case TftState::SETUP_SENDING: 
          TFT_UPDATE_SETUP();
          break;
        case TftState::SETUP_EXIT: 
          TFT_EXIT_SETUP();
          break;
        case TftState::UPDATE_EMCY: 
          TFT_EMCY();
          TFT_DSOC();  
          break;
        case TftState::SEND_CONFIRM: 
          TFT_SEND_CONFIRM();
          break;
        case TftState::SEND_DECLINE: 
          TFT_DECLINE_CONFIRM();
          break;
        case TftState::SEND_SUCCESS: 
          TFT_SEND_SUCCESS(); 
          break;
      }
      lastScene = static_cast<uint8_t>(scene); 
      timer = millis();
    }
  }
}

// Splash screen
void TFT_SETUP() 
{
  tft.init(); 
  tft.setRotation(1); 
  tft.setTextSize(2); 

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN); 
  tft.drawCentreString("CANopenNode", 160, 70, 4);

  tft.drawLine(20, 118, 300, 119, TFT_WHITE);

  tft.setTextSize(1);
  tft.setTextColor(TFT_YELLOW);
  tft.drawString("Simulator", 80, 130, 4); 
  
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Awaiting CAN...", 80, 180, 1);
}

// Update splash screen with current status
void TFT_UPDATE_SETUP()
{
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("CAN Received... ", 80, 180, 1);
}

// Exit splash screen 
void TFT_EXIT_SETUP()
{
  // rewrite previous screen, but in black to clear 
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK);
  tft.drawString("Simulator", 80, 130, 4); 
  tft.setTextSize(2); 
  tft.drawCentreString("CANopenNode", 160, 70, 4); 
  tft.drawLine(20, 118, 300, 119, TFT_BLACK);
  
  tft.drawString("CAN Received... ", 80, 180, 1);
  
  tft.setTextSize(2); 
  tft.setTextColor(TFT_YELLOW);
  tft.drawCentreString("Error Code:", 160, 65, 4);
}

// Display selected EMCY error code
void TFT_EMCY()
{
  static uint16_t last_error_code = 0xFFFF;
  uint16_t current_error_code = node.getEmcyCode();
  if (current_error_code != last_error_code)
  {
    last_error_code = current_error_code;

    char error_str[8] = {};

    memcpy(error_str, &current_error_code, sizeof(current_error_code));
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawCentreString(error_str, 160, 130, 2);
  }
}

// Display state of charge
void TFT_DSOC()
{
  char buf[16];
  static const uint32_t index = Object::findIndex(DISPLAYED_CHARGE, 0x01);
  sprintf(buf, "Charge: %u%%   ", dictionary[index].data[0]); 
  tft.setTextSize(1); 
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.drawString(buf, 0, 0, 4); 
}

// Display confirmation screen for sending EMCY
void TFT_SEND_CONFIRM()
{
  tft.setTextSize(1); 
  tft.setTextColor(0x94BD); // baby blue
  tft.drawCentreString("Send EMCY?", 160, 185, 4);
}

// Clear confirmation screen for sending EMCY
void TFT_DECLINE_CONFIRM()
{
  tft.setTextSize(1);
  tft.setTextColor(TFT_BLACK); 
  tft.drawCentreString("Send EMCY?", 160, 185, 4);
}

// Display success message after sending EMCY
void TFT_SEND_SUCCESS()
{
  char buf[16] = {};

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2); 
  tft.setTextColor(TFT_GREEN);
  tft.drawCentreString("EMCY Sent", 160, 80, 4);

  sprintf(buf, "Error Code: %04X", node.getEmcyCode());
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString(buf, 160, 140, 4);
}
/*
  jvmacro_usb.ino

  Receives button states from io mcu and sends keypresses to pc over usb.

  Uses NicoHood's HID Project library and HoodLoader2. 
  Flash Arduino Uno with Board -> HoodLoader2 16u2.

  Jos van Mourik (2021)
*/

#include "HID-Project.h"

// keyboard keys to map to buttons (ImprovedKeylayoutsUS.h)
static const KeyboardKeycode sKey[12][3] = {
  {0, 0, 0}, // S1 button
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0} // S12 button
};

// media keys to map to buttons (ConsumerAPI.h)
static const ConsumerKeycode sCon[12] = {
  0, // S1 button
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0, // S12 button
};

// initialize hid and serial
void setup() 
{
  NKROKeyboard.begin(); // keyboard keys init
  Consumer.begin(); // media keys init
  Serial1.begin(115200); // uart bridge to atmega328P
}

// read keys from io mcu and update hid
void loop() 
{ 
  // data ready, nice
  if (Serial1.read() == 69)
  {
    // wait till 12 key bytes are available
    while (Serial1.available() < 12);
    
    // received key states
    byte sState[12];
      
    // read keys
    Serial1.readBytes(sState, 12);
    
    // update pressed keys
    for (byte i = 0; i < 12; i++)
    {
      // update 3 keycodes per key
      for (byte j = 0; j < 3; j++)
      {  
        // update if keycode exists
        if (sKey[i][j])
        {
          if (sState[i]) NKROKeyboard.add(sKey[i][j]); // pressed
          else NKROKeyboard.release(sKey[i][j]); // released
        }
      }
        
      // update if media key exists
      if (sCon[i] and sState[i]) Consumer.write(sCon[i]);
    }
  
    // send pressed keys to pc
    NKROKeyboard.send();
  } 
}

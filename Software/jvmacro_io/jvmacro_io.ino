/*
  jvmacro_io.ino

  Polls all 12 Arduino Shield buttons and sends status to usb mcu.

  Uses NicoHood's HID Project library and HoodLoader2. 
  Flash Arduino Uno with Board -> HoodLoader2 Uno.

  Jos van Mourik (2021)
*/

byte sPin[12] = {5, 4, 3, 2, 9, 8, 7, 6, A0, A1, A2, A3}; // button io pin order
bool sFlag = false;        // key changed flag
byte sState_new[12] = {0}; // new key states for comparison 
byte sState_old[12] = {0}; // old key states for comparison

// initialize buttons and serial
void setup() 
{
  Serial.begin(115200); // serial bridge to usb mcu
  for (byte i = 0; i < 12; i++) pinMode(sPin[i], INPUT_PULLUP); // init shield io

  // turn off led
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

// read buttons and send values to usb mcu
void loop() 
{
  // poll all 12 buttons and check changes
  for (byte i = 0; i < 12; i++)
  {
    sState_new[i] = !digitalRead(sPin[i]);
    
    // check state changes and set flag if changed
    if (sState_new[i] != sState_old[i]) sFlag = true;
  }
  
  // button(s) changed: debounce and send to usb mcu 
  if (sFlag)
  {
    for (byte i = 0; i < 12; i++) sState_old[i] = sState_new[i]; // update history
    Serial.write(69); // data ready, nice
    Serial.write(sState_new, 12); // send states
    delay(25); // debounce
    sFlag = false;
  }
}

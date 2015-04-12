/*
  Madtoyslab. - Xbox Controller S breakout board V0.1a
  
  Debug sketch before lib-like implementation
  
  == ATmega328 pin mapping ==
  pin5/D3(PWM)   -> ljoyY PWM pin
  pin6/D4        -> Shift Register Dx
  pin11/D5(PWM)  -> Shift Register STCP
  pin12/D6(PWM)  -> Shift Register SHCP
  pin13/D7       -> ltrig PWM pin
  pin14/D8       -> rtrig PWM pin
  pin15/D9(PWM)  -> rjoyX PWM pin
  pin16/D10(PWM) -> rjoyY PWM pin
  pin17/D11(PWM) -> ljoyX PWM pin
  pin19/D13      -> status LED
  pin24/A1       -> rjoyX DIR pin
  pin25/A2       -> rjoyY DIR pin
  pin26/A3       -> ljoyX DIR pin
  pin28/A5       -> ljoyY DIR pin
  
  == 74HC595 n°1 pin mappin ==
  Q0 -> B Btn pin
  Q1 -> X Btn pin  
  Q2 -> A Btn pin
  Q3 -> Y Btn pin
  Q4 -> white Btn pin
  Q5 -> black Btn pin
  
  == 74HC595 n°2 pin mappin ==
  Q0 -> keypad right pin [def high]
  Q1 -> keypad left pin  [def high]
  Q2 -> keypad down pin  [def high]
  Q3 -> keypad up pin    [def high]
  Q4 -> start Btn pin    [def high]
  Q5 -> back Btn pin     [def high]
  Q6 -> rjoy SEL pin     [def high]
  Q7 -> ljoy SEL pin     [def high]
  
  StephaneAG - 2015
*/


// == ATmega328 pin mapping ==
const int ljoyY_PWM_pin = 3;   // pin5/D3(PWM)   -> ljoyY PWM pin
const int shRe_Dx_pin = 4;     // pin6/D4        -> Shift Register Dx ( data )
const int shRe_STCP_pin = 5;   // pin11/D5(PWM)  -> Shift Register STCP ( latch )
const int shRe_SHCP_pin = 6;   // pin12/D6(PWM)  -> Shift Register SHCP ( clock )
const int ltrig_pin = 7;       // pin13/D7       -> ltrig PWM pin
const int rtrig_pin = 8;       // pin14/D8       -> rtrig PWM pin
const int rjoyX_PWM_pin = 9;   // pin15/D9(PWM)  -> rjoyX PWM pin
const int rjoyY_PWM_pin = 10;  // pin16/D10(PWM) -> rjoyY PWM pin
const int ljoyX_PWM_pin = 11;  // pin17/D11(PWM) -> ljoyX PWM pin
const int statusLed_pin = 13;  // pin19/D13      -> status LED
const int rjoyX_DIR_pin = A1;  // pin24/A1       -> rjoyX DIR pin
const int rjoyY_DIR_pin = A2;  // pin25/A2       -> rjoyY DIR pin
const int ljoyX_DIR_pin = A3;  // pin26/A3       -> ljoyX DIR pin
const int ljoyY_DIR_pin = A4;  // pin28/A5       -> ljoyY DIR pin
// buffers
// triggers
int ltrig_chunk = 0; // updated by serial data
int rtrig_chunk = 0; // updated by serial data
// ljoy
int ljoyX_chunk = 0; // updated by serial data
int ljoyX_DIR = 0; // uberjoy's
int ljoyX_PWM = 0; // uberjoy's
int ljoyY_chunk = 0; // updated by serial data
int ljoyY_DIR = 0; // uberjoy's
int ljoyY_PWM = 0; // uberjoy's
// rjoy
int rjoyX_chunk = 0; // updated by serial data
int rjoyX_DIR = 0; // uberjoy's
int rjoyX_PWM = 0; // uberjoy's
int rjoyY_chunk = 0; // updated by serial data
int rjoyY_DIR = 0; // uberjoy's
int rjoyY_PWM = 0; // uberjoy's


byte byteBuffer1 = 0; // 'll hold our bits pattern for the first shift-register
// == 74HC595 n°1 pin mappin ==
int byteBuffer1_bBtn_chunk = 0;      // Q0 -> B Btn pin
int byteBuffer1_xBtn_chunk = 0;      // Q1 -> X Btn pin
int byteBuffer1_aBtn_chunk = 0;      // Q2 -> A Btn pin
int byteBuffer1_yBtn_chunk = 0;      // Q3 -> Y Btn pin
int byteBuffer1_whiteBtn_chunk = 0;  // Q4 -> white Btn pin
int byteBuffer1_blackBtn_chunk = 0;  // Q5 -> black Btn pin
int byteBuffer1_NA1_chunk = 0;       // Q6 -> N/A
int byteBuffer1_NA2_chunk = 0;       // Q7 -> N/A

byte byteBuffer2 = 0; // 'll hold our bits pattern for the second shift-register
// == 74HC595 n°2 pin mappin ==
int byteBuffer2_keypadRight_chunk = 0;  // Q0 -> keypad right pin
int byteBuffer2_keypadLeft_chunk = 0;   // Q1 -> keypad left pin
int byteBuffer2_keypadDown_chunk = 0;   // Q2 -> keypad down pin
int byteBuffer2_keypadUp_chunk = 0;     // Q3 -> keypad up pin
int byteBuffer2_startBtn_chunk = 0;     // Q4 -> start Btn pin
int byteBuffer2_backBtn_chunk = 0;      // Q5 -> back Btn pin
int byteBuffer2_rjoySel_chunk = 0;      // Q6 -> rjoy SEL pin
int byteBuffer2_ljoySel_chunk = 0;      // Q7 -> ljoy SEL pin


void setup(){
  Serial.begin(9600);
  
  // ATmega328 setup
  pinMode(statusLed_pin, OUTPUT);
  pinMode(ltrig_pin, OUTPUT);
  pinMode(rtrig_pin, OUTPUT);
  pinMode(ljoyX_DIR_pin, OUTPUT);
  pinMode(ljoyX_PWM_pin, OUTPUT);
  pinMode(ljoyY_DIR_pin, OUTPUT);
  pinMode(ljoyY_PWM_pin, OUTPUT);
  pinMode(rjoyX_DIR_pin, OUTPUT);
  pinMode(rjoyX_PWM_pin, OUTPUT);
  pinMode(rjoyY_DIR_pin, OUTPUT);
  pinMode(rjoyY_PWM_pin, OUTPUT);
  
  // shift-registers setup
  pinMode(shRe_Dx_pin, OUTPUT);
  pinMode(shRe_STCP_pin, OUTPUT);
  pinMode(shRe_SHCP_pin, OUTPUT);
  
  // uberjoys setup
  initUberJoy(ljoyX_DIR_pin, ljoyX_PWM_pin);
  initUberJoy(ljoyY_DIR_pin, ljoyY_PWM_pin);
  initUberJoy(rjoyX_DIR_pin, rjoyX_PWM_pin);
  initUberJoy(rjoyY_DIR_pin, rjoyY_PWM_pin);
  
  digitalWrite(statusLed_pin, HIGH);
  Serial.println("XboxControllerS - breakout board: Booted");
}


void loop(){
  handleSerial(); // handle serial data
}


void handleSerial(){
  while( Serial.available() > 0 ){
  
    // read from the serial conn & update skecthes' values / shift register's chunks
    // /!\ R: adjust the order of the calls to Serial.parseInt() to reflect the order of the Python Companion app
    // == direct values ==
    ltrig_chunk = Serial.parseInt();
    rtrig_chunk = Serial.parseInt();
    ljoyX_chunk = Serial.parseInt();
    ljoyY_chunk = Serial.parseInt();
    rjoyX_chunk = Serial.parseInt();
    rjoyY_chunk = Serial.parseInt();
    
    // == byte buffer n°1 ==
    byteBuffer1_bBtn_chunk = Serial.parseInt();
    byteBuffer1_xBtn_chunk = Serial.parseInt();
    byteBuffer1_aBtn_chunk = Serial.parseInt();
    byteBuffer1_yBtn_chunk = Serial.parseInt();
    byteBuffer1_whiteBtn_chunk = Serial.parseInt();
    byteBuffer1_blackBtn_chunk = Serial.parseInt();
    // == byte buffer n°2 ==
    byteBuffer2_keypadRight_chunk = Serial.parseInt();
    byteBuffer2_keypadLeft_chunk = Serial.parseInt();
    byteBuffer2_keypadDown_chunk = Serial.parseInt();
    byteBuffer2_keypadUp_chunk = Serial.parseInt();
    byteBuffer2_startBtn_chunk = Serial.parseInt();
    byteBuffer2_backBtn_chunk = Serial.parseInt();
    byteBuffer2_rjoySel_chunk = Serial.parseInt();
    byteBuffer2_ljoySel_chunk = Serial.parseInt();
    
    // look for a line ending
    if( Serial.read() == '\n' ){
      // update values / shift registers' chunks
      updateTriggers();
      updateJoysXY();
      updateShiftingBits();
      // update the shift registers
      updateShiftRegisters();
      // printout callback
      printSerialCallback();
    }
  }
  //delay(3000); // blocking debug delay
}


// R: no analog-like behavior is yet implemented 
void updateTriggers(){
  if( ltrig_chunk == 1 ){ digitalWrite(ltrig_pin, HIGH); } else { digitalWrite(ltrig_pin, LOW); }
  if( rtrig_chunk == 1 ){ digitalWrite(rtrig_pin, HIGH); } else { digitalWrite(rtrig_pin, LOW); }
}


// R: constrain can be used to stay in the 0-255 range
// = theorically / expected by Xbox Controller S =
// fully forward    4.6V
// nothing          2.3V
// fully backward   0.0V
void updateJoysXY(){
  handleJoystickAxis( ljoyX_chunk, ljoyX_DIR_pin, ljoyX_PWM_pin );
  handleJoystickAxis( ljoyY_chunk, ljoyY_DIR_pin, ljoyY_PWM_pin );
  handleJoystickAxis( rjoyX_chunk, rjoyX_DIR_pin, rjoyX_PWM_pin );
  handleJoystickAxis( rjoyY_chunk, rjoyY_DIR_pin, rjoyY_PWM_pin );
}


void updateShiftingBits(){
  // == byte buffer n°1 ==
  bitWrite(byteBuffer1, 0, byteBuffer1_bBtn_chunk);      // Q0 -> B Btn pin
  bitWrite(byteBuffer1, 1, byteBuffer1_xBtn_chunk);      // Q1 -> X Btn pin
  bitWrite(byteBuffer1, 2, byteBuffer1_aBtn_chunk);      // Q2 -> A Btn pin
  bitWrite(byteBuffer1, 3, byteBuffer1_yBtn_chunk);      // Q3 -> Y Btn pin
  bitWrite(byteBuffer1, 4, byteBuffer1_whiteBtn_chunk);  // Q4 -> white Btn pin
  bitWrite(byteBuffer1, 5, byteBuffer1_blackBtn_chunk);  // Q5 -> black Btn pin
  // R: 1st shift register' Q6 & Q7 are not used/connected, and thus are set but never updated 
  // == byte buffer n°2 ==
  //if( byteBuffer2_keypadRight_chunk == 0 ){ bitSet(byteBuffer2, 0); } else { bitClear(byteBuffer2, 0); }
  bitWrite(byteBuffer1, 0, !byteBuffer2_keypadRight_chunk);  // Q0 -> keypad right pin
  bitWrite(byteBuffer1, 1, !byteBuffer2_keypadLeft_chunk);   // Q1 -> keypad left pin
  bitWrite(byteBuffer1, 2, !byteBuffer2_keypadDown_chunk);   // Q2 -> keypad down pin
  bitWrite(byteBuffer1, 3, !byteBuffer2_keypadUp_chunk);     // Q3 -> keypad up pin
  bitWrite(byteBuffer1, 4, !byteBuffer2_startBtn_chunk);     // Q4 -> start Btn pin
  bitWrite(byteBuffer1, 5, !byteBuffer2_backBtn_chunk);      // Q5 -> back Btn pin
  bitWrite(byteBuffer1, 6, !byteBuffer2_rjoySel_chunk);      // Q6 -> rjoy SEL pin
  bitWrite(byteBuffer1, 7, !byteBuffer2_ljoySel_chunk);      // Q7 -> ljoy SEL pin
}


// R: check if needed to shift out the shift registers in the inverse order ( aka 2nd then 1st )
void updateShiftRegisters(){
  // = shift register n°1 =
  digitalWrite(shRe_STCP_pin, LOW); //prepare to write - latch pin LOW
  shiftOut(shRe_Dx_pin, shRe_SHCP_pin, MSBFIRST, byteBuffer1); // LSBFIRST = data from byte from bit 0 to bit 7
  // R: here we're using MSBFIRST to have the byte bits indexes 0 to 7 matches outputs indexes Q0 to Q7
  digitalWrite(shRe_STCP_pin, HIGH);
  // = shift register n°2 =
  digitalWrite(shRe_STCP_pin, LOW); //prepare to write
  shiftOut(shRe_Dx_pin, shRe_SHCP_pin, MSBFIRST, byteBuffer2);
  digitalWrite(shRe_STCP_pin, HIGH);
}


void printSerialCallback(){
  // triggers
  Serial.print("ltrig:"); if( ltrig_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); }
  Serial.print("rtrig:"); if( rtrig_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); }
  // joysticks
  Serial.print("ljoyY:"); Serial.print(ljoyY_chunk); Serial.print("/ljoyX:"); Serial.print(ljoyX_chunk);
  Serial.print("/ljoyS:"); if( byteBuffer2_ljoySel_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); } // [def high]
  Serial.print("rjoyY:"); Serial.print(rjoyY_chunk); Serial.print("/rjoyX:"); Serial.print(rjoyX_chunk);
  Serial.print("/rjoyS:"); if( byteBuffer2_rjoySel_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); } // [def high]
  // buttons
  Serial.print("btnBack:");  if( byteBuffer2_backBtn_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); } // [def high]
  Serial.print("btnStart:");  if( byteBuffer2_startBtn_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); } // [def high]
  Serial.print("btnBlack:"); if( byteBuffer1_blackBtn_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); }
  Serial.print("btnWhite:"); if( byteBuffer1_whiteBtn_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); }
  Serial.print("btnY:"); if( byteBuffer1_yBtn_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); }
  Serial.print("btnA:"); if( byteBuffer1_aBtn_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); }
  Serial.print("btnX:"); if( byteBuffer1_xBtn_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); }
  Serial.print("btnB:"); if( byteBuffer1_bBtn_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); }
  // Keypad
  Serial.print("keypadUp:");   if( byteBuffer2_keypadUp_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); }// [def high]
  Serial.print("keypadDown:");  if( byteBuffer2_keypadDown_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); } // [def high]
  Serial.print("keypadLeft:");  if( byteBuffer2_keypadLeft_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); } // [def high]
  Serial.print("keypadRight:");  if( byteBuffer2_keypadRight_chunk == 1 ){ Serial.print("on/"); } else { Serial.print("off/"); } // [def high]
  Serial.println("");
}


void initUberJoy(int dirPin, int pwmPin){
  digitalWrite(dirPin, LOW);
  analogWrite(pwmPin, 0);
}


// /!\ R: below code is still in debugging & uses the V0.1a version of the uberjoy breakouts
// accept a value from 0 to 256 & 2 pins to handle a joystick axis
// - maps the value to a direction & a pwm value
// - updates the 2 pins to reflect the value passed
void handleJoystickAxis(int value, int dirPin, int pwmPin){
  if( value == 128 ){ 
    Serial.print("value is 128 => don't source nor sink .."); Serial.println("");
    analogWrite(pwmPin, 0); // whatever the direction, don't source nor sink
  } else if( value > 128 && value <= 256 ) { // forward => source
    Serial.print("value greater than 128 => sourcing .."); Serial.println("");
    digitalWrite( dirPin, LOW ); // source
    analogWrite( pwmPin, map( value, 128, 256, 256, 0 ) ); // 128~256 => 0~256 for pwm forward
  } else if( value < 128 && value >= 0 ) { // backward => sink
    Serial.print("value smaller than 128 => sinking .."); Serial.println("");
    digitalWrite( dirPin, HIGH ); // sink
    analogWrite( pwmPin, map( value, 0, 128, 256, 0 ) ); // 0~128 => 256~0 for pwm backward
  }
}

/*
    Madtoyslab_XboxControllerS.h
    
    Madtoyslab_XboxControllerS - library for the Xbox Controller S breakout board by Madtoyslab.
    
    The breakout simulate a player's controls, using hardware wirings & circuitry to manipulate the signals goin to the Xobx Controller's onboard uC
    It bypasses the Xbox check for "valid" Xbox controllers as to the Xbox Controller's onboard uC, the inputs are coming from the standard controls
    
    Allows to plug any controller ( keyboard, mouse, Arduino, custom, .. ) to an Xbox with unlimited "controls mapping" possibilities
    
    R: 
    if you just wanna use an Xbox Controller S on a laptop, just cut the controller's cable & rewire the wires for USB ( lefting one alone unused )
    the breakout does the opposite, aka allows to control an Xbox from a laptop as it'd be a native Xbox Controller [ nb: actually passing signals from the laptop "through" it ;p ]
    
    Created by Stephane ADAM GARNIER - 2015
    Released under the MIT license
    
    version: 0.01a
*/


/* act according to Arduino's flags */
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
# else
  #include "WProgram.h"
#endif


/* include our library header file */
#include "Madtoyslab_XboxControllerS.h"



/* the actual library content(s) */
// constructor
Madtoyslab_XboxControllerS::Madtoyslab_XboxControllerS(int ledpin){
  pinMode(ledpin, OUTPUT);
  _ledpin = ledpin;
  _nextBlinkInterval = 1000;
  _lastBLinkTriggerTime = 0;
  _blinkState = 0;
  _breakoutBlinkState = _breakoutLastBlinkState = 1; // we show the booting blinking pattern on startup
  _dummyCounter = 0;
}

// init
void Madtoyslab_XboxControllerS::init(){
  Serial.begin(9600);
  Serial.println("XboxControllerS [lib V0.01a] - breakout board: Booted");
}


// loop
void Madtoyslab_XboxControllerS::loop(){
  _handleBlink();
}


/* activity LED on D13 */
// non-blocking triggering of the blink update
void Madtoyslab_XboxControllerS::_handleBlink(){
  if( (unsigned long) millis() - _lastBLinkTriggerTime >= _nextBlinkInterval ){ // if the set interval of milliseconds elapsed
    _lastBLinkTriggerTime = millis(); // update the last moment the set interval of milliseconds elapsed
    _updateBlink(); // trigger the blink state & update the D13 LED / the next milliseconds interval
  }
}

// blink update
// TODO: modify the below function to check the type of blink currently set & then either init it if it differs from the previous one or continue it if it didn't change
void Madtoyslab_XboxControllerS::_updateBlink(){
  _timedBlinkMap();
  /*
  if(_blinkState == 0){ // toggle the D13 LED ON for 250ms / aka wait for 250ms
    digitalWrite(_ledpin, HIGH);
    _nextBlinkInterval = 200;
    _blinkState = 1;
  } else if(_blinkState == 1){ // toggle the D13 LED OFF for 500ms / aka wait for 500ms
    digitalWrite(_ledpin, LOW);
    _nextBlinkInterval = 200;
    _blinkState = 2;
  } else if(_blinkState == 2){ // toggle the D13 LED ON for 250ms / aka wait for 250ms
    digitalWrite(_ledpin, HIGH);
    _nextBlinkInterval = 200;
    _blinkState = 3;
  } else if(_blinkState == 3){ // toggle the D13 LED OFF for 1000ms / aka wait for 1000ms to complete its "blink loop"
    digitalWrite(_ledpin, LOW);
    _nextBlinkInterval = 1000;
    _blinkState = 0;
    Serial.println("debug loop ..");
  }
  */
  
  // change the blink sequence after a while ['d be after some user input]
  if(_dummyCounter == 100){ // go from booting sequence blinking to working fine blinking 
    _breakoutBlinkState = 0;
    _dummyCounter++; // quickly prevent repeating useless Serial.print any further
    Serial.println("blinking boot sequence done !");
  }
  else if ( _dummyCounter < 100) {
    _dummyCounter++;
    Serial.println("Booting countdown :"); Serial.println(100-_dummyCounter);
  }
}

// blink update usng "timed blink map" pattern
void Madtoyslab_XboxControllerS::_timedBlinkMap(){
  // handle switching between sequence of blinking
  if( _breakoutBlinkState != _breakoutLastBlinkState ){ // start at beginning of new blinking sequence if a change in the breakout state occured
    _breakoutLastBlinkState = _breakoutBlinkState;
    //Serial.print(" value: "); Serial.print(__timedBlinkMaps[_breakoutBlinkState][i][0]);
    digitalWrite(_ledpin, _timedBlinkMaps[_breakoutBlinkState][0][0]);
    //Serial.print(" next interval: "); Serial.print(_timedBlinkMap_default[i][1]);
    _nextBlinkInterval = _timedBlinkMaps[_breakoutBlinkState][0][1];
    //Serial.print(" next state: "); Serial.println(_timedBlinkMap_default[i][2]);
    _blinkState = _timedBlinkMaps[_breakoutBlinkState][0][2];
  } 
  // anyway, continue current blinking sequence where it left off 
  for (int i = 0; i < (int)(sizeof(_timedBlinkMaps[_breakoutBlinkState])/sizeof(_timedBlinkMaps[_breakoutBlinkState][0])); i++) {
    if( (int)_blinkState == i ){
      //Serial.print("state "); Serial.print(i); Serial.print(": ");
      //Serial.print(" value: "); Serial.print(_timedBlinkMap_default[i][0]);
      digitalWrite(_ledpin, _timedBlinkMaps[_breakoutBlinkState][i][0]);
      //Serial.print(" next interval: "); Serial.print(_timedBlinkMap_default[i][1]);
      _nextBlinkInterval = _timedBlinkMaps[_breakoutBlinkState][i][1];
      //Serial.print(" next state: "); Serial.println(_timedBlinkMap_default[i][2]);
      _blinkState = _timedBlinkMaps[_breakoutBlinkState][i][2];
      // if we wish to keep some Serial print or other call when the last blink pattern has ended
      if( _blinkState == 0 ) Serial.println("blink sequence loop ..");
      return;
    }
  }
    
  

}
  
/*
// blink update usng "timed blink map" pattern
void Madtoyslab_XboxControllerS::_timedBlinkMap(){
  //Serial.println("loopOverTimedBlinkPattern called !");
  for (int i = 0; i < (int)(sizeof(_timedBlinkMap_default)/sizeof(_timedBlinkMap_default[0])); i++) {
    if( (int)_blinkState == i ){
      //Serial.print("state "); Serial.print(i); Serial.print(": ");
      //Serial.print(" value: "); Serial.print(_timedBlinkMap_default[i][0]);
      digitalWrite(_ledpin, _timedBlinkMap_default[i][0]);
      //Serial.print(" next interval: "); Serial.print(_timedBlinkMap_default[i][1]);
      _nextBlinkInterval = _timedBlinkMap_default[i][1];
      //Serial.print(" next state: "); Serial.println(_timedBlinkMap_default[i][2]);
      _blinkState = _timedBlinkMap_default[i][2];
      // if we wish to keep some Serial print or other call when the last blink pattern has ended
      if( _blinkState == 0 ) Serial.println("debug loop ..");
      return;
    }
  }
}
*/

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


/* include our library header file */
#include "Madtoyslab_XboxControllerS.h"


/* instance(s) of the library */
Madtoyslab_XboxControllerS xboxController(13); // pass pin D13


void setup(){
  xboxController.init();
}


void loop(){
  xboxController.loop();
}

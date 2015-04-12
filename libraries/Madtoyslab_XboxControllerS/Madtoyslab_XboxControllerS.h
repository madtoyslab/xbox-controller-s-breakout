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


/* prevent multiple inclusions - start */
#ifndef Madtoyslab_XboxControllerS_h
#define Madtoyslab_XboxControllerS_h


/* act according to Arduino's flags */
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
# else
  #include "WProgram.h"
#endif



/* the actual library content(s) */

// some patterns for the LED blink wich provides a quick visual feedback on the internal state of the board/product
//static int _timedBlinkMap_default[][3] = { {HIGH, 100, 1}, {LOW, 100, 2}, {HIGH, 100, 3}, {LOW, 1000, 0} };
/*
static int _timedBlinkMaps_working[][3] = { {HIGH, 100, 1}, {LOW, 1000, 2}, {HIGH, 100, 3}, {LOW, 1000, 0} };
static int _timedBlinkMaps_booting[][3] = { {HIGH, 50, 1}, {LOW, 50, 2}, {HIGH, 50, 3}, {LOW, 100, 0} };
static int _timedBlinkMaps[][3] = { _timedBlinkMaps_working, _timedBlinkMaps_booting };
*/

static int _timedBlinkMaps[][4][3] = { 
                                      { {HIGH, 100, 1}, {LOW, 2000, 2}, {HIGH, 100, 3}, {LOW, 2000, 0} }, // working fine
                                      { {HIGH, 50, 1}, {LOW, 50, 2}, {HIGH, 50, 3}, {LOW, 100, 0} }, // booting
                                      { {HIGH, 600, 1}, {LOW, 300, 2}, {HIGH, 600, 3}, {LOW, 1000, 0} }, // error
                                      // ...
                                    };  

                                    
/* TODO: add patterns for:
                           - working fine ( = default, so done ;) )
                           - booting
                           - error
                           - connecting to laptop software
                           - laptop software connected
                           - upgrading firmware
                           - firmware upgraded
                           [ the two below could be used for calibration purposes ]
                           - waiting <step>
                           - <step> validated
*/

// the main class
class Madtoyslab_XboxControllerS
{
  public:
    Madtoyslab_XboxControllerS(int pin);
    void init();
    void loop();
    void dot();
    void dash();
    
    
  private:
    /* activity LED on D13 */
    int _ledpin; // D13 LED pin
    int _breakoutBlinkState; // the state of the breakout ( provided as visual feedback to the user via the D13 LED )
    int _breakoutLastBlinkState;
    unsigned long _nextBlinkInterval; // = 1000; // how much milliseconds to wait for next blink trigger & state handling
    unsigned long _lastBLinkTriggerTime; // = 0; // millis representation of how much time elapsed since last blink trigger & state handling
    int _blinkState; // = 0; // the current blink state
    void _handleBlink(); // schedules the blink triggers regarding to the next blink interval set
    void _updateBlink(); // update the blink state, the D13 LED as well as the next milliseconds interval
    void _timedBlinkMap(); // update the D13 LED blinking using a "timed blink map" ( multidimensional array )
    
    int _dummyCounter; // used to change the blinking pattern after a while ( simulates initialisation of stuff / sequence change after user input )
};


/* prevent multiple inclusions - end */
#endif // #ifndef Madtoyslab_XboxControllerS_h

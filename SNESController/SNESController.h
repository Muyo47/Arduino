/* Library for the SNES controller */


#ifndef SNESController_H
#define SNESController_H

#include <Arduino.h>


class SNESController {
public:
    SNESController (int LatchPin, int ClockPin, int DataPin); // Latch pin, Clock pin, Data pin
    void begin ();   // Initialization
    void readData (uint8_t data[16]);   // To read data, provide the function a 16 positions vector


private:
    uint8_t _LatchPin; // Latch pin definition
    uint8_t _ClockPin;  // Clock pin definition
    uint8_t _DataPin;  // Data pin definition

    enum State { IDLE, e1, e2, e3, e4 };    // States for the FSM of the SNES controller
    State currentState;                     // Transition between states

    bool flagData;
    uint8_t buttons[16];                          // Data array
};

#endif
/* Library for the TB6612FNG. Includes options for 1 motor */


#ifndef MotorDriver_H
#define MotorDriver_H

#include <Arduino.h>

class Driver {
public:
    Driver (int stbypin, int a0pin, int a1pin, int pwmpin); // Constructor
    void begin (); // Initialization
    void moveForward (int direction, float speed); // Methods
    void softStop ();
    void hardStop ();

private:
    uint8_t _stbyPin; // Standby pin definition
    uint8_t _a0Pin;  // A0 pin definition
    uint8_t _a1Pin;  // A1 pin definition
    uint8_t _PWMAPin;    // PWM pin definition
};

#endif
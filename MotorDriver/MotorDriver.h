/* Library for the TB6612FNG. Includes options for 1 motor */


#ifndef MotorDriver_H
#define MotorDriver_H

#include <Arduino.h>

#ifndef MULTIPLE_MOTORS
#define MULTIPLE_MOTORS 1           // Set this to 0 if you want to use only one motor (by default motor A)
#endif                              // You can also use one single motor with MULTIPLE_MOTORS option enabled,
                                    // but the I/O pins consumption will increase since you need to declare them


class Driver {
public:
#if MULTIPLE_MOTORS
    Driver (int stbypin, int a0pin, int a1pin, int pwmApin, int b0pin, int b1pin, int pwmBpin); // Constructor
    void begin (); // Initialization
    void spinMotor (int motor, int direction, float speed); // Methods
    void turn (int turnDirection, float speed);
    void move (int direction, float speed);
    void singleStop (int motor);          // Single stop only stop one motor while letting the second motor maintain the state
    void fullStop ();                     // Full stop sets standby pin on 0, stopping both motors
#else
    Driver (int stbypin, int a0pin, int a1pin, int pwmApin); // Constructor
    void begin (); // Initialization
    void spinMotor (int direction, float speed); // Methods
    void singleStop ();
#endif

private:
    uint8_t _stbyPin; // Standby pin definition
    uint8_t _a0Pin;  // A0 pin definition
    uint8_t _a1Pin;  // A1 pin definition
    uint8_t _PWMAPin;    // PWM pin definition

    #if MULTIPLE_MOTORS
        // New pins for motor B
        uint8_t _b0Pin;  // B0 pin definition
        uint8_t _b1Pin;  // B1 pin definition
        uint8_t _PWMBPin;    // PWM pin definition
    #endif
};

#endif
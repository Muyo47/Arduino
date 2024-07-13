#include "MotorDriver.h"

#define CLOCKWISE 1
#define COUNTERCLOCKWISE 0
#define MOTOR_A 0
#define MOTOR_B 1

#if MULTIPLE_MOTORS
    Driver::Driver(int stbypinA, int a0pin, int a1pin, int pwmApin, int stbypinB, int b0pin, int b1pin, int pwmBpin) {
        _stbyPinA = stbypinA;
        _a0Pin = a0pin;
        _a1Pin = a1pin;
        _PWMAPin = pwmApin;
        _stbyPinB = stbypinB;
        _b0Pin = b0pin;
        _b1Pin = b1pin;
        _PWMBPin = pwmBpin;
    }

#else
    Driver::Driver(int stbypinA, int a0pin, int a1pin, int pwmApin) {
        _stbyPinA = stbypinA;
        _a0Pin = a0pin;
        _a1Pin = a1pin;
        _PWMAPin = pwmApin;
    }
#endif

#if MULTIPLE_MOTORS
    void Driver::begin() {

        pinMode(_stbyPinA, OUTPUT);
        pinMode(_a0Pin, OUTPUT);
        pinMode(_a1Pin, OUTPUT);
        pinMode(_PWMAPin, OUTPUT);
        pinMode(_stbyPinB, OUTPUT);
        pinMode(_b0Pin, OUTPUT);
        pinMode(_b1Pin, OUTPUT);
        pinMode(_PWMBPin, OUTPUT);
    }
#else
    void Driver::begin() {

        pinMode(_stbyPinA, OUTPUT);
        pinMode(_a0Pin, OUTPUT);
        pinMode(_a1Pin, OUTPUT);
        pinMode(_PWMAPin, OUTPUT);
    }
#endif


#if MULTIPLE_MOTORS

    void Driver::moveForward(int motor, int direction, float speed) {
        if (speed > 255){
            Serial.println ("Speed is too high. Reducing it to the max speed value admitted");
            speed = 255;
        }

        if (speed < 0){
            Serial.println ("No negative speed values are admitted. Changing speed to 0");
            speed = 0;
        }

        if(motor == MOTOR_A){

            if (direction == CLOCKWISE){
                digitalWrite(_stbyPinA, 1);
                digitalWrite(_a1Pin, 1);
                digitalWrite(_a0Pin, 0);
                analogWrite(_PWMAPin, speed);
            }

            if (direction == COUNTERCLOCKWISE){
                digitalWrite(_stbyPinA, 1);
                digitalWrite(_a1Pin, 0);
                digitalWrite(_a0Pin, 1);
                analogWrite(_PWMAPin, speed);
            }

            if (direction != CLOCKWISE && direction != COUNTERCLOCKWISE){
                Serial.println("Wrong direction request, autoselection of clockwise direction");

                digitalWrite(_stbyPinA, 1);
                digitalWrite(_a1Pin, 1);
                digitalWrite(_a0Pin, 0);
                analogWrite(_PWMAPin, speed);
            }
        }

        if(motor == MOTOR_B){

            if (direction == CLOCKWISE){
                digitalWrite(_stbyPinB, 1);
                digitalWrite(_b1Pin, 1);
                digitalWrite(_b0Pin, 0);
                analogWrite(_PWMBPin, speed);
            }

            if (direction == COUNTERCLOCKWISE){
                digitalWrite(_stbyPinB, 1);
                digitalWrite(_b1Pin, 0);
                digitalWrite(_b0Pin, 1);
                analogWrite(_PWMBPin, speed);
            }

            if (direction != CLOCKWISE && direction != COUNTERCLOCKWISE){
                Serial.println("Wrong direction request, autoselection of clockwise direction");

                digitalWrite(_stbyPinB, 1);
                digitalWrite(_b1Pin, 1);
                digitalWrite(_b0Pin, 0);
                analogWrite(_PWMBPin, speed);
            }
        }

        if(motor != MOTOR_A && motor != MOTOR_B){
            Serial.println("Unable to select the desired motor. Motor A will be selected. Make sure you choose the correct motor!");

            if (direction == CLOCKWISE){
                digitalWrite(_stbyPinA, 1);
                digitalWrite(_a1Pin, 1);
                digitalWrite(_a0Pin, 0);
                analogWrite(_PWMAPin, speed);
            }

            if (direction == COUNTERCLOCKWISE){
                digitalWrite(_stbyPinA, 1);
                digitalWrite(_a1Pin, 0);
                digitalWrite(_a0Pin, 1);
                analogWrite(_PWMAPin, speed);
            }

            if (direction != CLOCKWISE && direction != COUNTERCLOCKWISE){
                Serial.println("Wrong direction request, autoselection of clockwise direction");

                digitalWrite(_stbyPinA, 1);
                digitalWrite(_a1Pin, 1);
                digitalWrite(_a0Pin, 0);
                analogWrite(_PWMAPin, speed);
            }
        }
    }


#else
    void Driver::moveForward(int direction, float speed) {
        if (speed > 255){
            Serial.println ("Speed is too high. Reducing it to the max speed value admitted");
            speed = 255;
        }

        if (speed < 0){
            Serial.println ("No negative speed values are admitted. Changing speed to 0");
            speed = 0;
        }

        if (direction == CLOCKWISE){
            digitalWrite(_stbyPinA, 1);
            digitalWrite(_a1Pin, 1);
            digitalWrite(_a0Pin, 0);
            analogWrite(_PWMAPin, speed);
        }

        if (direction == COUNTERCLOCKWISE){
            digitalWrite(_stbyPinA, 1);
            digitalWrite(_a1Pin, 0);
            digitalWrite(_a0Pin, 1);
            analogWrite(_PWMAPin, speed);
        }

        if (direction != CLOCKWISE || direction != COUNTERCLOCKWISE){
            Serial.println("Wrong direction request, autoselection of clockwise direction");

            digitalWrite(_stbyPinA, 1);
            digitalWrite(_a1Pin, 1);
            digitalWrite(_a0Pin, 0);
            analogWrite(_PWMAPin, speed);
        }

    }
#endif


#if MULTIPLE_MOTORS

void Driver::softStop(int motor) {
    if (motor == MOTOR_A){
        digitalWrite(_stbyPinA, 1);
        digitalWrite(_a1Pin, 0);
        digitalWrite(_a0Pin, 0);
        analogWrite(_PWMAPin, 0);
    }

    if (motor == MOTOR_B){
        digitalWrite(_stbyPinB, 1);
        digitalWrite(_b1Pin, 0);
        digitalWrite(_b0Pin, 0);
        analogWrite(_PWMBPin, 0);
    }

    if (motor != MOTOR_A && motor != MOTOR_B){

        Serial.println("Motor selection is invalid. Stopping both motors");

        digitalWrite(_stbyPinA, 1);
        digitalWrite(_a1Pin, 0);
        digitalWrite(_a0Pin, 0);
        analogWrite(_PWMAPin, 0);

        digitalWrite(_stbyPinB, 1);
        digitalWrite(_b1Pin, 0);
        digitalWrite(_b0Pin, 0);
        analogWrite(_PWMBPin, 0);
    }
}

void Driver::hardStop(int motor) {
    if (motor == MOTOR_A){
        digitalWrite(_stbyPinA, 0);
        digitalWrite(_a1Pin, 0);
        digitalWrite(_a0Pin, 0);
        analogWrite(_PWMAPin, 0);
    }

    if (motor == MOTOR_B){
        digitalWrite(_stbyPinB, 0);
        digitalWrite(_b1Pin, 0);
        digitalWrite(_b0Pin, 0);
        analogWrite(_PWMBPin, 0);
    }

    if (motor != MOTOR_A && motor != MOTOR_B){

        Serial.println("Motor selection is invalid. Stopping both motors");

        digitalWrite(_stbyPinA, 0);
        digitalWrite(_a1Pin, 0);
        digitalWrite(_a0Pin, 0);
        analogWrite(_PWMAPin, 0);

        digitalWrite(_stbyPinB, 0);
        digitalWrite(_b1Pin, 0);
        digitalWrite(_b0Pin, 0);
        analogWrite(_PWMBPin, 0);
    }
    
}

#else

void Driver::softStop() {
    digitalWrite(_stbyPinA, 1);
    digitalWrite(_a1Pin, 0);
    digitalWrite(_a0Pin, 0);
    analogWrite(_PWMAPin, 0);
}

void Driver::hardStop() {
    digitalWrite(_stbyPinA, 0);
    digitalWrite(_a1Pin, 0);
    digitalWrite(_a0Pin, 0);
    analogWrite(_PWMAPin, 0);
}

#endif
#include "MotorDriver.h"

#define CLOCKWISE 1
#define COUNTERCLOCKWISE 0

Driver::Driver(int stbypin, int a0pin, int a1pin, int pwmpin) {
    _stbyPin = stbypin;
    _a0Pin = a0pin;
    _a1Pin = a1pin;
    _PWMAPin = pwmpin;
}

void Driver::begin() {

    pinMode(_stbyPin, OUTPUT);
    pinMode(_a0Pin, OUTPUT);
    pinMode(_a1Pin, OUTPUT);
    pinMode(_PWMAPin, OUTPUT);
}

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
        digitalWrite(_stbyPin, 1);
        digitalWrite(_a1Pin, 1);
        digitalWrite(_a0Pin, 0);
        analogWrite(_PWMAPin, speed);
    }

    if (direction == COUNTERCLOCKWISE){
        digitalWrite(_stbyPin, 1);
        digitalWrite(_a1Pin, 0);
        digitalWrite(_a0Pin, 1);
        analogWrite(_PWMAPin, speed);
    }

    if (direction != CLOCKWISE && direction != COUNTERCLOCKWISE){
        Serial.println("Wrong direction request, autoselection of clockwise direction");

        digitalWrite(_stbyPin, 1);
        digitalWrite(_a1Pin, 1);
        digitalWrite(_a0Pin, 0);
        analogWrite(_PWMAPin, speed);
    }
    
}

void Driver::softStop() {
    digitalWrite(_stbyPin, 1);
    digitalWrite(_a1Pin, 0);
    digitalWrite(_a0Pin, 0);
    analogWrite(_PWMAPin, 0);
}

void Driver::hardStop() {
    digitalWrite(_stbyPin, 0);
    digitalWrite(_a1Pin, 0);
    digitalWrite(_a0Pin, 0);
    analogWrite(_PWMAPin, 0);
    
}
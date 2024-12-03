#include "MotorDriver.h"

enum spinModes{
    CLOCKWISE = 1,
    COUNTERCLOCKWISE = 0
};
enum motors{
    MOTOR_A = 0,
    MOTOR_B = 1
};
enum turnDirections{
    RIGHT_DIRECTION = 0,
    LEFT_DIRECTION = 1
};
enum moveDirections{
    FORWARD = 0,
    BACKWARDS = 1
};


#if MULTIPLE_MOTORS
    Driver::Driver(int stbypin, int a0pin, int a1pin, int pwmApin, int b0pin, int b1pin, int pwmBpin) {
        _stbyPin = stbypin;
        _a0Pin = a0pin;
        _a1Pin = a1pin;
        _PWMAPin = pwmApin;
        _b0Pin = b0pin;
        _b1Pin = b1pin;
        _PWMBPin = pwmBpin;
    }

#else
    Driver::Driver(int stbypin, int a0pin, int a1pin, int pwmApin) {
        _stbyPin = stbypin;
        _a0Pin = a0pin;
        _a1Pin = a1pin;
        _PWMAPin = pwmApin;
    }
#endif

#if MULTIPLE_MOTORS
    void Driver::begin() {

        pinMode(_stbyPin, OUTPUT);
        pinMode(_a0Pin, OUTPUT);
        pinMode(_a1Pin, OUTPUT);
        pinMode(_PWMAPin, OUTPUT);
        pinMode(_b0Pin, OUTPUT);
        pinMode(_b1Pin, OUTPUT);
        pinMode(_PWMBPin, OUTPUT);
    }
#else
    void Driver::begin() {

        pinMode(_stbyPin, OUTPUT);
        pinMode(_a0Pin, OUTPUT);
        pinMode(_a1Pin, OUTPUT);
        pinMode(_PWMAPin, OUTPUT);
    }
#endif


#if MULTIPLE_MOTORS

    void Driver::spinMotor(int motor, int direction, float speed) {
        if (speed > 255){
            Serial.println ("Speed too high. Reducing it to 255");
            speed = 255;
        }

        else if (speed < 0){
            Serial.println ("Negative speed not allowed. Setting it to 0");
            speed = 0;
        }

        if(motor == MOTOR_A){

            if (direction == CLOCKWISE){
                digitalWrite(_stbyPin, HIGH);
                digitalWrite(_a1Pin, HIGH);
                digitalWrite(_a0Pin, LOW);
                analogWrite(_PWMAPin, speed);
            }

            else if (direction == COUNTERCLOCKWISE){
                digitalWrite(_stbyPin, HIGH);
                digitalWrite(_a1Pin, LOW);
                digitalWrite(_a0Pin, HIGH);
                analogWrite(_PWMAPin, speed);
            }

            else {
                Serial.println("Wrong direction request, motor stopped");

                digitalWrite(_stbyPin, LOW);
                digitalWrite(_a1Pin, LOW);
                digitalWrite(_a0Pin, LOW);
            }
        }

        else if(motor == MOTOR_B){

            if (direction == CLOCKWISE){
                digitalWrite(_stbyPin, HIGH);
                digitalWrite(_b1Pin, HIGH);
                digitalWrite(_b0Pin, LOW);
                analogWrite(_PWMBPin, speed);
            }

            else if (direction == COUNTERCLOCKWISE){
                digitalWrite(_stbyPin, HIGH);
                digitalWrite(_b1Pin, LOW);
                digitalWrite(_b0Pin, HIGH);
                analogWrite(_PWMBPin, speed);
            }

            else {
                Serial.println("Wrong direction request, motor stopped");

                digitalWrite(_stbyPin, LOW);
                digitalWrite(_b1Pin, LOW);
                digitalWrite(_b0Pin, LOW);
                analogWrite(_PWMBPin, speed);
            }
        }

        else {
            Serial.println("Unable to select the desired motor. No motor will be selected. Make sure you choose the correct motor!");

                digitalWrite(_stbyPin, LOW);
                digitalWrite(_a1Pin, LOW);
                digitalWrite(_a0Pin, LOW);
                digitalWrite(_b1Pin, LOW);
                digitalWrite(_b0Pin, LOW);
        }
    }

    void Driver::turn(int turnDirection, float speed) {
        if (speed > 255){
            Serial.println ("Speed too high. Reducing it to 255");
            speed = 255;
        }

        else if (speed < 0){
            Serial.println ("Negative speed not allowed. Setting it to 0");
            speed = 0;
        }


        if (turnDirection == RIGHT_DIRECTION){
            digitalWrite(_stbyPin, HIGH);

            digitalWrite(_a1Pin, HIGH);
            digitalWrite(_a0Pin, LOW);
            analogWrite(_PWMAPin, speed);

            digitalWrite(_b1Pin, LOW);
            digitalWrite(_b0Pin, HIGH);
            analogWrite(_PWMBPin, speed);
        }

        else if (turnDirection == LEFT_DIRECTION){
            digitalWrite(_stbyPin, HIGH);

            digitalWrite(_a1Pin, LOW);
            digitalWrite(_a0Pin, HIGH);
            analogWrite(_PWMAPin, speed);

            digitalWrite(_b1Pin, HIGH);
            digitalWrite(_b0Pin, LOW);
            analogWrite(_PWMBPin, speed);
        }

        else {
            Serial.println ("Set '0' for right turn or '1' for left turn. Different values are not admitted");
            digitalWrite(_stbyPin, LOW);

            digitalWrite(_a1Pin, LOW);
            digitalWrite(_a0Pin, LOW);

            digitalWrite(_b1Pin, LOW);
            digitalWrite(_b0Pin, LOW);
        }
    }


    void Driver::move(int direction, float speed) {
        if (speed > 255){
            Serial.println ("Speed too high. Reducing it to 255");
            speed = 255;
        }

        else if (speed < 0){
            Serial.println ("Negative speed not allowed. Setting it to 0");
            speed = 0;
        }


        if (direction == FORWARD){
            digitalWrite(_stbyPin, HIGH);

            digitalWrite(_a1Pin, LOW);
            digitalWrite(_a0Pin, HIGH);
            analogWrite(_PWMAPin, speed);

            digitalWrite(_b1Pin, LOW);
            digitalWrite(_b0Pin, HIGH);
            analogWrite(_PWMBPin, speed);
        }

        else if (direction == BACKWARDS){
            digitalWrite(_stbyPin, HIGH);

            digitalWrite(_a1Pin, HIGH);
            digitalWrite(_a0Pin, LOW);
            analogWrite(_PWMAPin, speed);

            digitalWrite(_b1Pin, HIGH);
            digitalWrite(_b0Pin, LOW);
            analogWrite(_PWMBPin, speed);
        }

        else {
            Serial.println ("Set '0' for forward or '1' for backward. Different values are not admitted");
            digitalWrite(_stbyPin, LOW);

            digitalWrite(_a1Pin, LOW);
            digitalWrite(_a0Pin, LOW);

            digitalWrite(_b1Pin, LOW);
            digitalWrite(_b0Pin, LOW);
        }
        
    }


#else
    void Driver::spinMotor(int direction, float speed) {
        if (speed > 255){
            Serial.println ("Speed too high. Reducing it to 255");
            speed = 255;
        }

        if (speed < 0){
            Serial.println ("Negative speed not allowed. Setting it to 0");
            speed = 0;
        }

        if (direction == CLOCKWISE){
            digitalWrite(_stbyPin, 1);
            digitalWrite(_a1Pin, 1);
            digitalWrite(_a0Pin, 0);
            analogWrite(_PWMAPin, speed);
        }

        else if (direction == COUNTERCLOCKWISE){
            digitalWrite(_stbyPin, 1);
            digitalWrite(_a1Pin, 0);
            digitalWrite(_a0Pin, 1);
            analogWrite(_PWMAPin, speed);
        }

        else {
            Serial.println("Wrong direction request, motor will be stopped");

            digitalWrite(_stbyPin, LOW);
            digitalWrite(_a1Pin, LOW);
            digitalWrite(_a0Pin, LOW);
        }

    }
#endif


#if MULTIPLE_MOTORS

void Driver::singleStop(int motor) {
    if (motor == MOTOR_A){
        digitalWrite(_stbyPin, HIGH);
        digitalWrite(_a1Pin, LOW);
        digitalWrite(_a0Pin, LOW);
        analogWrite(_PWMAPin, 0);
    }

    else if (motor == MOTOR_B){
        digitalWrite(_stbyPin, HIGH);
        digitalWrite(_b1Pin, LOW);
        digitalWrite(_b0Pin, LOW);
        analogWrite(_PWMBPin, 0);
    }

    else {

        Serial.println("Motor selection is invalid. Stopping both motors");

        digitalWrite(_stbyPin, HIGH);

        digitalWrite(_a1Pin, LOW);
        digitalWrite(_a0Pin, LOW);
        analogWrite(_PWMAPin, 0);

        digitalWrite(_b1Pin, LOW);
        digitalWrite(_b0Pin, LOW);
        analogWrite(_PWMBPin, 0);
    }
}

void Driver::fullStop() {

        digitalWrite(_stbyPin, LOW);

        digitalWrite(_a1Pin, LOW);
        digitalWrite(_a0Pin, LOW);
        analogWrite(_PWMAPin, 0);
        digitalWrite(_b1Pin, LOW);
        digitalWrite(_b0Pin, LOW);
        analogWrite(_PWMBPin, 0);
}

#else

void Driver::singleStop() {
    digitalWrite(_stbyPin, LOW);
    digitalWrite(_a1Pin, LOW);
    digitalWrite(_a0Pin, LOW);
    analogWrite(_PWMAPin, LOW);
}

#endif
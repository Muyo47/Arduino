/* Library for tb6612fng DC motor driver. Follow the conexions given in the example. Make sure
to give power to the motor with an external power source (the amount of current a DC motor can draw
overcomes the max current an average development board can give)

This example is for two motors.

Make sure you set #define MULTIPLE_MOTORS as 1 in MotorDriver.h
This code has been tested in an ESP32 WROOM32
*/

#include "MotorDriver.h"

#define STANDBY_PIN 26

#define A0_PIN 25       // Definitions for motor A pins
#define A1_PIN 33
#define PWMA_PIN 32

#define B0_PIN 18       // Definitions for motor B pins
#define B1_PIN 19
#define PWMB_PIN 21

enum libraryDef{
  CLOCKWISE = 1,
  COUNTERCLOCKWISE = 0,   // Directions (library only admits 0 and 1 as parameters, make sure you only use "1" or "0")

  MOTOR_A = 0,      
  MOTOR_B = 1,             // Motor selection (library only admits 0 and 1 as parameters, make sure you only use "1" or "0")

  RIGHT_DIRECTION = 0,
  LEFT_DIRECTION = 1,

  FORWARD = 0,
  BACKWARD = 1
};


Driver driver(STANDBY_PIN, A0_PIN, A1_PIN, PWMA_PIN, B0_PIN, B1_PIN, PWMB_PIN);    //Create object and pass all pins required

void setup() {
  Serial.begin(115200);     // For debugging
  driver.begin();           // Begin driver
}

void loop() {
  driver.spinMotor(MOTOR_A, CLOCKWISE, 200);   // 1 for clockwise, 0 for counterclockwise. Speed can reach up to 255
  driver.spinMotor(MOTOR_B, COUNTERCLOCKWISE, 50);
  delay(2000);
  driver.singleStop(MOTOR_A);  // Stop only motor A
  delay(2000);
  driver.spinMotor(MOTOR_B, CLOCKWISE, 100);
  driver.spinMotor(MOTOR_A, COUNTERCLOCKWISE, 75);
  delay(2000);
  driver.fullStop();        // Stop both motors
  delay(2000);
  driver.turn(LEFT_DIRECTION, 140);
  delay(2000);
  driver.fullStop();
  delay(2000);
  driver.move(FORWARD, 210)
  delay(2000);
}
      // Highly recommended using fullStop (or singleStop) between movement transitions
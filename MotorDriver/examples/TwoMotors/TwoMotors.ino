/* Library for tb6612fng DC motor driver. Follow the conexions given in the example. Make sure
to give power to the motor with an external power source (the amount of current a DC motor can draw
overcomes the max current an esp32 can give)

This example is for two motors.

Make sure you set #define MULTIPLE_MOTORS as 1 in MotorDriver.h

*/

#include "MotorDriver.h"

#define STANDBYA_PIN 26   // Definitions for motor A pins
#define A0_PIN 25
#define A1_PIN 33
#define PWMA_PIN 32


#define STANDBYB_PIN 20   // Definitions for motor B pins
#define B0_PIN 21
#define B1_PIN 22
#define PWMB_PIN 23


#define CLOCKWISE 1           // Directions (library only admits 0 and 1 as parameters, make sure you only use "1" or "0")
#define COUNTERCLOCKWISE 0

#define MOTOR_A 0             // Motor selection (library only admits 0 and 1 as parameters, make sure you only use "1" or "0")
#define MOTOR_B 1

Driver driver(STANDBYA_PIN, A0_PIN, A1_PIN, PWMA_PIN, STANDBYB_PIN, B0_PIN, B1_PIN, PWMB_PIN);    //Create object and pass all pins required

void setup() {
  Serial.begin(115200);     //For debugging
  driver.begin();           //Begin driver
}

void loop() {
  driver.moveForward(MOTOR_A, CLOCKWISE, 200);   // 1 for clockwise, 0 for counterclockwise. Speed can reach to 255
  delay(2000);
  driver.softStop(MOTOR_A);
  delay(2000);
  driver.moveForward(MOTOR_B, COUNTERCLOCKWISE, 100);
  delay(2000);
  driver.hardStop(MOTOR_B);
  delay(2000);
}

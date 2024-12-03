/* Library for tb6612fng DC motor driver. Follow the conexions given in the example. Make sure
to give power to the motor with an external power source (the amount of current a DC motor can draw
overcomes the max current an average development board can give)

This example is for one motor. To see the example for two motors, check "TwoMotors" example.
Code tested with ESP32 WROOM32

Make sure you set #define MULTIPLE_MOTORS as 0 in MotorDriver.h
If #define MULTIPLE_MOTORS 1 code won't work!

*/

#include "MotorDriver.h"

#define STANDBYA_PIN 26   // Definitions for motor A pins
#define A0_PIN 25
#define A1_PIN 33
#define PWMA_PIN 32

#define CLOCKWISE 1           // Directions (library only admits 0 and 1 as parameters, make sure you only use "1" or "0")
#define COUNTERCLOCKWISE 0

Driver driver(STANDBYA_PIN, A0_PIN, A1_PIN, PWMA_PIN);    //Create object and pass all pins required

void setup() {
  Serial.begin(115200);     //For debugging
  driver.begin();           //Begin driver
}

void loop() {
  driver.spinMotor(CLOCKWISE, 200);   // 1 for clockwise, 0 for counterclockwise. Speed can reach up to 255
  delay(2000);
  driver.stop();
  delay(2000);
  driver.spinMotor(COUNTERCLOCKWISE, 100);
  delay(2000);
  driver.singleStop();
  delay(2000);
}

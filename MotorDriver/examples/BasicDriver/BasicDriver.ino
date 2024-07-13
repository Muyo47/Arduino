/* Library for tb6612fng DC motor driver. Follow the conexions given in the example. Make sure
to give power to the motor with an external power source (the amount of current a DC motor can draw
overcomes the max current an esp32 can give)

CURRENTLY LIBRARY CAN HOLD UP TO ONLY MOTOR A. DO NOT TRY TO USE IT FOR MOTOR A & B.
*/

#include "MotorDriver.h"

#define STANDBY_PIN 26
#define A0_PIN 25
#define A1_PIN 33
#define PWM_PIN 32
#define CLOCKWISE 1
#define COUNTERCLOCKWISE 0

Driver driver(STANDBY_PIN, A0_PIN, A1_PIN, PWM_PIN);    //Create object and pass all pins required

void setup() {
  Serial.begin(115200);     //For debugging
  driver.begin();           //Begin driver
}

void loop() {
  driver.moveForward(CLOCKWISE, 200);   // 1 for clockwise, 0 for counterclockwise. Speed can reach to 255
  delay(2000);
  driver.softStop();
  delay(2000);
  driver.moveForward(COUNTERCLOCKWISE, 100);
  delay(2000);
  driver.hardStop();
  delay(2000);
}

#include "SNESController.h"


    SNESController::SNESController(int LatchPin, int ClockPin, int DataPin){
        _LatchPin = LatchPin;
        _ClockPin = ClockPin;
        _DataPin = DataPin; 
    }
    

    void SNESController::begin() {

        pinMode(_LatchPin, OUTPUT);
        pinMode(_ClockPin, OUTPUT);
        pinMode(_DataPin, INPUT);

        currentState = IDLE;
    }


void SNESController::readData(uint8_t data[16]) {
    unsigned long currentMicros = micros();
    static unsigned long previousMicros = 0;
    const unsigned int latchPulseWidth = 12;      
    const unsigned int clockPulseWidth = 6;       

    static uint8_t shiftRegister = 0;

    flagData = false;

    while (flagData == false) {
        currentMicros = micros();
        //Serial.println("Dentro de la maquina de estados");  //para debugguear el codigo
        
        switch (currentState) {
            case IDLE:
                //Serial.println("Estado: IDLE");
                if (currentMicros - previousMicros >= latchPulseWidth) {
                    shiftRegister = 0;
                    currentState = e1;
                    previousMicros = currentMicros;
                }
                else currentState = IDLE;
                break;

            case e1:
                //Serial.println("Estado: e1");
                if (currentMicros - previousMicros >= latchPulseWidth) {
                    currentState = e2;
                    previousMicros = currentMicros;
                }
                else currentState = e1;
                break;

            case e2:
                //Serial.println("Estado: e2");
                if (currentMicros - previousMicros > clockPulseWidth) {
                    currentState = e3;
                    previousMicros = currentMicros;
                }
                else currentState = e2;
                break;

            case e3:
                //Serial.println("Estado: e3");
                if (currentMicros - previousMicros > clockPulseWidth) {
                    if (shiftRegister == 15 ){
                        currentState = e4;                                  
                        previousMicros = currentMicros;
                    } else {
                        currentState = e2;
                        buttons[shiftRegister] = digitalRead(_DataPin);
                        shiftRegister++;                                  
                        previousMicros = currentMicros;
                    }
                }
                else currentState = e3;
                break;

            case e4:
                //Serial.println("Estado: e4");
                if (currentMicros - previousMicros > latchPulseWidth) {
                    flagData = true;
                    currentState = IDLE;
                    previousMicros = currentMicros;
                }
                else currentState = e4;
                break;
        }

        // Set outputs here
        switch (currentState) {
            case IDLE:
                digitalWrite(_LatchPin, LOW);
                digitalWrite(_ClockPin, HIGH);
                break;
            case e1:
                digitalWrite(_LatchPin, HIGH);
                digitalWrite(_ClockPin, HIGH);
                break;
            case e2:
                digitalWrite(_LatchPin, LOW);
                digitalWrite(_ClockPin, HIGH);
                break;
            case e3:
                digitalWrite(_LatchPin, LOW);
                digitalWrite(_ClockPin, LOW);
                break;
            case e4:
                digitalWrite(_LatchPin, LOW);
                digitalWrite(_ClockPin, HIGH);
                break;
        }
    }
    

    for (int i = 0; i < 16; i++) {
        data[i] = !buttons[i]; // Copy the button states to the provided array
    }
}

/* Example to read and print SNES controller pressed buttons. Tested with an ESP32 WROOM32 */
/* B[0]   Y[1]   SELECT[2]   START[3]   UP[4]   DOWN[5]   LEFT[6]   RIGHT[7]   A[8]   X[9]   L[10]   R[11]   ?[12]   ?[13]   ?[14]   ?[15] */

#include "SNESController.h"

#define LATCH_PIN 5
#define CLOCK_PIN 18
#define DATA_PIN 19

uint8_t buttonStates[16];   // Array to store the button states

SNESController snesController(LATCH_PIN, CLOCK_PIN, DATA_PIN);

void setup() {
    Serial.begin(115200); 
    snesController.begin(); // Init controller
    Serial.print("If button state '1' button is NOT pressed\n");
}

void loop() {

    snesController.readData(buttonStates);    // Read all controller data
    for (int i = 0; i < 16; i++) {
        Serial.print("Button ");
        Serial.print(i);
        Serial.print(" : ");
        Serial.print(buttonStates[i]);
        Serial.print(" ");
    }
    
    Serial.println();
    delay(7500);
}
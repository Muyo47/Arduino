## SNES CONTROLLER

Library for the **SNES controller**. 
To understand how library works, check out the given example.
The button possition into the array is commented in the example code. readData function **only read once**.
If you change the pinout, make sure the I/O **data** pin **can _read_**, explore pinout of you development board.
| Function | Param | Description |
|-----:|-----------|-----------|
| SNESController | LatchPin, ClockPin, DataPin | Instance and give Pinout |
| Begin | -- | Init |
| readData | int </yourVariableName/>[16] | Write into the variable the buttons state |

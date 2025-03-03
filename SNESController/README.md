## SNES CONTROLLER

Library for the **SNES controller**. 
To understand how library works, check out the example provided.
The button possition into the array is commented in the example code. readData function **reads only once**.
If you change the pinout, make sure the I/O **data** pin **can _read_**, refer to your development board's pinout.
| Function | Param | Description |
|-----:|-----------|-----------|
| SNESController | LatchPin, ClockPin, DataPin | Instance and give Pinout |
| Begin | -- | Init |
| readData | int </yourVariableName/>[16] | Write into the variable the buttons state |
<br>
<br>
<br>
<br>

<picture>
 <source media="(prefers-color-scheme: dark)" srcset="https://consolemods.org/wiki/images/1/16/SNES_Controller_Female.svg">
 <source media="(prefers-color-scheme: light)" srcset="https://consolemods.org/wiki/images/1/16/SNES_Controller_Female.svg">
 <img alt="SNES CONTROLLER PINOUT" src="https://consolemods.org/wiki/images/1/16/SNES_Controller_Female.svg">
</picture>

| Pin | Name |
|-----:|-----------|
| 1 | 5V (can work at 3.3V too) |
| 2 | Clock |
| 3 | Latch |
| 4 | Data |
| 5 | N/C |
| 6 | N/C |
| 7 | Ground |

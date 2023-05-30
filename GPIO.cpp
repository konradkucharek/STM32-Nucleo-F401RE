// Program: GPIO control of LED based on a provided table

/*
---------------------------------------------
| LED | 0-3 | 4-5 | 6-7 | 8-11 | 12-14 | 15 |
---------------------------------------------
| RL  | ON  | OFF | ON  | OFF  |  OFF  | ON |
---------------------------------------------
| GL  | OFF | ON  | ON  | ON   |  OFF  | ON |
---------------------------------------------
| BL  | OFF | OFF | OFF | ON   |  ON   | ON |
---------------------------------------------
*/

#include "mbed.h"

#define SLEEP_TIME     100ms

// BUTTON 1 is LSB
// BUTTON 4 is MSB
#define BUTTON_1 D2  // PA_10
#define BUTTON_2 D3  // PB_3
#define BUTTON_3 D4  // PB_5
#define BUTTON_4 D5  // PB_4

#define RED_LED D6   // PB_10
#define GREEN_LED D7 // PA_8
#define BLUE_LED D8  // PA_9

// Define the Input pins 
// DigitalIn format: DigitalIn mybutton (Input Pin);
DigitalIn b1(BUTTON_1);
DigitalIn b2(BUTTON_2);
DigitalIn b3(BUTTON_3);
DigitalIn b4(BUTTON_4);

// Define the Output pins
// DigitalOut format: DigitalOut Led_out (Output Pin);
DigitalOut RL (RED_LED);
DigitalOut GL (GREEN_LED);
DigitalOut BL (BLUE_LED);

BusIn buttonsbus (BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4);
BusOut ledsbus (BLUE_LED, GREEN_LED, RED_LED);

// Define Input/Output buses
void ControlLED_DigitalIO ();
void ControlLED_BusIO ();

int main()
{
    while(1) 
	{
        ControlLED_DigitalIO ();
        ThisThread::sleep_for(SLEEP_TIME);
    }
}

void ControlLED_DigitalIO ()
{
    // Bool expressions
    // Logic derived via Karnaugh Map of project requirements
    RL = (!b4 && !b3) || (!b4 && b2) || (b3 && b2 && b1);
    GL = (!b4 && b3) || (b4 && !b3) || (b3 && b2 && b1);
    BL = b4;
}

void ControlLED_BusIO ()
{
    switch (buttonsbus)
    {
		// LED cases for 0 - 15
		case 0 ... 3:
			ledsbus=0b0100;
			break;
		case 4 ... 5:
			ledsbus=0b0010;
			break;
		case 6 ... 7:
			ledsbus=0b0110;
			break;
		case 8 ... 11:
			ledsbus=0b0011;
			break;
		case 12 ... 14:
			ledsbus=0b0001;
			break;
		case 15:
			ledsbus=0b0111;
			break;
    }
}
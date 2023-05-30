/*----------------------------------------------------------------------------
Program: INTERRUPT IN/OUT - Counters
PROGRAMMING USING MBED API
----------------------------------------
This program counts the number of times a button has been pressed and 
lights up the RGB LED. This will correspond to the button which has been pressed the most.
    •	Button 1 corresponds to the Red LED
    •	Button 2 corresponds to the Green LED
    •	Button 3 corresponds to the Blue LED
    •	Button 4 is used to reset all the counters and turn off all the LEDs.
Also, if several buttons have been pressed the same number of times their respective LEDs should all be ON simultaneously. 
----------------------------------------------------------------------------*/
 
#include "mbed.h"
#include <iostream>

using namespace std;

#define BUTTON_1 D2 
#define BUTTON_2 D3 
#define BUTTON_3 D4 
#define BUTTON_4 D5 

#define RED_LED D6
#define GREEN_LED D7
#define BLUE_LED D8

// Define Outputs
DigitalOut rLED(RED_LED);
DigitalOut gLED(GREEN_LED);
DigitalOut bLED(BLUE_LED);

// Define Interrupt Inputs
InterruptIn b1_press(BUTTON_1);
InterruptIn b2_press(BUTTON_2);
InterruptIn b3_press(BUTTON_3);
InterruptIn b4_press(BUTTON_4);

// Define & Initialize Counters
volatile int b1C = 0, b2C = 0, b3C = 0;

void updater()
{

    ThisThread::sleep_for(250ms);

    rLED = (b1C) && (b1C >= b2C && b1C >= b3C);
    gLED = (b2C) && (b2C >= b1C && b2C >= b3C);
    bLED = (b3C) && (b3C >= b1C && b3C >= b2C);

}

// Define ISRs for the interrupts
void button_1_handler()
{
	b1C++;
}

void button_2_handler()
{
	b2C++;
}

void button_3_handler()
{
	b3C++;
}

void button_4_handler()
{
	b1C = 0, b2C = 0, b3C = 0;
    rLED = 0, gLED = 0, bLED = 0;
}

int main()
{
	// Enable Interrupts
	__enable_irq();
	
	// Initially turn off all LEDs
    b1C = b2C = b3C = 0;
    rLED = gLED = bLED = 0;
	
	// Interrupt handlers
    b1_press.fall(&button_1_handler);
    b2_press.fall(&button_2_handler);
    b3_press.fall(&button_3_handler);
    b4_press.fall(&button_4_handler);
	
	while(1)
    {
		__WFI();
        printf("b1C = %d\n", b1C);
        printf("b2C = %d\n", b2C);
        printf("b3C = %d\n", b3C);
        updater();
        ThisThread::sleep_for(1s);
	}
}
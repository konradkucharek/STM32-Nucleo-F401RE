/*----------------------------------------------------------------------------
Program: INTERRUPT IN/OUT
PROGRAMMING USING MBED API
 ----------------------------------------
Use the Mbed API functions to:
 
	1) Define InterruptIn and ISR for each button press
	2) In the interrupt service routine, the RGB LED is used to indicate when a
	button was pressed:
			+ Button 1 - light RED
			+ Button 2 - light BLUE
			+ Button 3 - light GREEN
			+ Button 4 - light WHITE (RED, GREEN and BLUE at the same time)
			
	3) Put the processor into sleep mode upon exiting from the ISR
 *----------------------------------------------------------------------------*/
 
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

// Define ISRs for the interrupts
void button_1_handler()
{
	rLED = !rLED;
}

void button_2_handler()
{
	gLED = !gLED;
}

void button_3_handler()
{
	bLED = !bLED;
}

void button_4_handler()
{
    rLED = gLED = bLED = 1;
}

void updater()
{
    ThisThread::sleep_for(250ms);
}

int main()
{
	
	__enable_irq(); // Enable interrupts
	
	// Initially turn off all LEDs
    rLED = gLED = bLED = 0;
	
	// Interrupt handlers
    b1_press.rise(&button_1_handler);
    b2_press.rise(&button_2_handler);
    b3_press.rise(&button_3_handler);
    b4_press.rise(&button_4_handler);

	
	while(1)
    {
		__WFI();
    }
}
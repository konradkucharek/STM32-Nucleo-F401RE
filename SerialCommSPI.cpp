/*
SERIAL COMMUNICATION [SPI]
----------------------------------------
Interface the LCD display using SPI protocol
*/

#include "mbed.h"
#include "NHD_0216HZ.h"
#include <iostream>

#define String1 "Hello"
#define String2 "Goodbye"

int main() 
{  
    // Intialize the SPI and the LCD 
    init_spi();
    init_lcd();
	
    // Print the first string
    print_lcd(String1);

    // Set the cursor to (0,1)
    set_cursor(0,1);

    // Print the second string
    print_lcd(String2);

    while (1)
    {
        // Started with inits and 1 print. no set cursor
        
        __WFE();
    }
}
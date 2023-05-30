// UART Program

#include "mbed.h"

// Create Serial Port
Serial pc(USBTX, USBRX);

char c_to_upper(char c) {
    if (c >= 'a' && c <= 'z') {
        c -= 0x20; 
    }
    return c;
}

int main()
{
    // Set Baud Rate
    pc.baud(9600);
    // Print "Hello to the world of mbed!"
    pc.puts("Hello to the world of mbed!\n");
    // Type some lowercase letters and echo their corresponding uppercase letters
    while(1)
    {
        pc.printf("%c\n", c_to_upper(pc.getc()));
    }
}
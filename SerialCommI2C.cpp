// I2C Program

#include "mbed.h"

// Define I2C and serial interfaces: pc and temp_sensor
Serial pc(USBTX, USBRX);
I2C temp_sensor(I2C_SDA, I2C_SCL);

// I2C address of temperature sensor
const int temp_addr = 0x90;

// Declare array to hold needed commands: convert T command and read temperature command 
char commands[2] = {0x51, 0xAA};
char commands2[1] = {0xAA};

// Declare array to hold 16-bit temperature data (MSByte and LSByte)
char temperature[2] = {0x0, 0x0};

int main()
{
	while(true)
    {
		// Write the Start Convert T command to the sensor and wait 500 ms
        temp_sensor.write(temp_addr, commands, 2);
        ThisThread::sleep_for(500);

		// Write the Read Temperature command to the sensor
        temp_sensor.write(temp_addr, commands2, 1);

		// Read the 16-bit temperature data
        temp_sensor.read(temp_addr, temperature, 2);
		
		// Convert temperature to Celsius
        float tempInC = 0.0625 * (((temperature[0] << 8) + temperature[1]) >> 4);
		
		// Display temperature on the serial monitor
        pc.printf("%f", tempInC);
        pc.printf(" C\n");
	}
}
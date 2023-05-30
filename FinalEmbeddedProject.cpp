// Final Project

#include "mbed.h"
#include "rtos.h"
#include "XNucleoIKS01A3.h"
#include <iostream>
#include "platform/mbed_thread.h"

// Blinking rate in milliseconds
#define blink 150

// Instantiate the expansion board
static XNucleoIKS01A3 *mems_expansion_board = XNucleoIKS01A3::instance(D14, D15, D4, D5, A3, D6, A4);

// Retrieve the composing elements of the expansion board
static LIS2MDLSensor *magnetometer = mems_expansion_board->magnetometer;
static HTS221Sensor *hum_temp = mems_expansion_board->ht_sensor;
static LPS22HHSensor *press_temp = mems_expansion_board->pt_sensor;
static LSM6DSOSensor *acc_gyro = mems_expansion_board->acc_gyro;
static LIS2DW12Sensor *accelerometer = mems_expansion_board->accelerometer;
static STTS751Sensor *temp = mems_expansion_board->t_sensor;

// Define and/or initialize variables
uint8_t id;
float value1, value2;
float convert = 0.0;
volatile int flag = 0;
int counter = 0;

// Button Input
#define button_user USER_BUTTON
InterruptIn user_press(button_user);


// Defining DigitalOut
DigitalOut led11(PA_6);  // D12
DigitalOut led12(PB_6);  // D10
DigitalOut led13(PC_7);  // D9
DigitalOut led21(PB_3);  // D3
DigitalOut led22(PB_5);  // D4
DigitalOut led23(PB_4);  // D5
DigitalOut led31(PB_10); // D6
DigitalOut led32(PA_8);  // D7
DigitalOut led33(PA_9);  // D8

DigitalOut ledR(PC_0); // A5
DigitalOut ledG(PC_1); // A4

/* LED Layout Visualization Matrix (Viewed down on LED bulbs from above)
[
    11 12 13
    21 22 23
    31 32 33
]
*/

void leds()
{
    ledR = 0;
    ledG = 1;
}
    
// Turns off all LEDs
void turn_off()
{
    led11 = led12 = led13 = 0;
    led21 = led22 = led23 = 0;
    led31 = led32 = led33 = 0;
}

// Turns on all LEDs
void turn_on()
{
    led11 = led12 = led13 = 1;
    led21 = led22 = led23 = 1;
    led31 = led32 = led33 = 1;
}

// Toggles LED state
void WarningBlink()
{
    led11 = led12 = led13 = !led11;
    led21 = led22 = led23 = !led21;
    led31 = led32 = led33 = !led31;
}



// Rotating Blinker
void RotBlink()
{
    while(true){
        led22 = 1;
        led11 = led33 = 1;
        wait_us(blink*1000);
        led11 = led33 = 0;
        led12 = led32 = 1;
        wait_us(blink*1000);
        led12 = led32 = 0;
        led13 = led31 = 1;
        wait_us(blink*1000);
        led13 = led31 = 0;
        led21 = led23 = 1;
        wait_us(blink*1000);
        led21 = led23 = 0;
    }
}

// Snake Game
void snake()
{
    led31 = 0;
    led13 = 1;
    wait_us(blink*1000);
    led13 = 0;
    led23 = 1;
    wait_us(blink*1000);
    led23 = 0;
    led33 = 1;
    wait_us(blink*1000);
    led33 = 0;
    led32 = 1;
    wait_us(blink*1000);
    led32 = 0;
    led22 = 1;
    wait_us(blink*1000);
    led22 = 0;
    led12 = 1;
    wait_us(blink*1000);
    led12 = 0;
    led11 = 1;
    wait_us(blink*1000);
    led11 = 0;
    led21 = 1;
    wait_us(blink*1000);
    led21 = 0;
    led31 = 1;
}

//------------------------- Gyro Function (LSM6DSO 3-axis accelero & gyro) ---------------------------

void Angle()
{
    leds();
    int32_t axes[3];
    turn_off();
    while(true){
        acc_gyro->get_x_axes(axes);
        printf("LSM6DSO [gyro/mdps]:   %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);

        if (axes[0] > 300) // forward
            {led13 = led23 = led33 = !led13;
            ThisThread::sleep_for(blink);}
        if (axes[0] < -300) // back
            {led11 = led21 = led31 = !led11;
            ThisThread::sleep_for(blink);}
        if (axes[1] > 300) // left
            {led11 = led12 = led13 = !led11;
            ThisThread::sleep_for(blink);}
        if (axes[1] < -300) // right
            {led31 = led32 = led33 = !led31;
            ThisThread::sleep_for(blink);}
        if (axes[2] < 500 || axes[2] > 1500)
            snake();
        else
            turn_off();
    } 
}

void MoveInCube()
{
    leds();
    while(true)
	{
		int32_t axes[3];
		turn_off();
		printf("\n | Level 1 [EASY] |\n");
		printf("\n >> Make a cross with the lights!\n");
		wait_us(500000);
		while(!(led23 && led21 && led12 && led32))
		{
			acc_gyro->get_x_axes(axes);
			led22 = !led22;
			wait_us(blink*1000);
			// printf("LSM6DSO [gyro/mdps]:   %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);
			if (axes[0] > 300) // forward
				led23 = 1;
			if (axes[0] < -300) // back
				led21 = 1;
			if (axes[1] > 300) // left
				led12 = 1;
			if (axes[1] < -300) // right
				led32 = 1;
		}
		wait_us(200000);
		printf("\n | Level 1 complete |\n");
		printf("-----------------------------");
		wait_us(200000);
		turn_off();
		printf("\n | Level 2 [MEDIUM (actually not)] |\n");
		printf("\n >> Make a square with the lights!\n");
		wait_us(500000);
		while(!(led23 && led33 && led13 && led12 && led11 && led21 && led31 && led32))
		{
			acc_gyro->get_x_axes(axes);
			led22 = !led22;
			wait_us(blink*1000);
			// printf("LSM6DSO [gyro/mdps]:   %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);
		
			if (axes[0] > 300 && -300<axes[1] && axes[1]<300) // forward
				led23 = 1;
			if (axes[0] < -300 && -300<axes[1] && axes[1]<300) // back
				led21 = 1;
			if (axes[1] > 300 && -300<axes[0] && axes[0]<300) // left
				led12 = 1;
			if (axes[1] < -300 && -300<axes[0] && axes[0]<300) // right
				led32 = 1;
			if (axes[0] > 300 && axes[1] > 300) // forward left
				led13 = 1;
			if (axes[0] > 300 && axes[1] < -300) // forward right
				led33 = 1;
			if (axes[0] < -300 && axes[1] > 300) // back left
				led11 = 1;
			if (axes[0] < -300 && axes[1] < -300) // back right
				led31 = 1;
		}
		wait_us(200000);
		printf("\n | Level 2 complete |\n");
		printf("-----------------------------");
		wait_us(200000);
		turn_off();
		printf("\n | Level 3 [HARD] |\n");
		printf("\n >> Make a X with the lights!\n");
		wait_us(500000);
		while(!(led31 && led11 && led31 && led13))
		{
			acc_gyro->get_x_axes(axes);
			led22 = !led22;
			wait_us(blink*1000);
			if (axes[0] > 300 && axes[1] > 300) // forward left
				led13 = 1;
			if (axes[0] > 300 && axes[1] < -300) // forward right
				led33 = 1;
			if (axes[0] < -300 && axes[1] > 300) // back left
				led11 = 1;
			if (axes[0] < -300 && axes[1] < -300) // back right
				led31 = 1;
		}
		wait_us(200000);
		printf("\n | Level 3 complete (wasn't hard actually lol) |\n");
		printf("-----------------------------");
		wait_us(200000);
		turn_off();
    }
}


void Hello()
{
    while(true)
	{
    //H
    led13 = led23 = led33 = led22 = led11 = led21 = led31 = 1;
    ThisThread::sleep_for(250);
    led13 = led23 = led33 = led22 = led11 = led21 = led31 = 0;
    ThisThread::sleep_for(250);
    //E
    led31 = led32 = led33 = led23 = led22 = led13 = led12 = led11 = 1;
    ThisThread::sleep_for(250);
    led31 = led32 = led33 = led23 = led22 = led13 = led12 = led11 = 0;
    ThisThread::sleep_for(250);
    //L
    led33 = led23 = led13 = led12 = led11 = 1;
    ThisThread::sleep_for(250);
    led33 = led23 = led13 = led12 = led11 = 0;
    ThisThread::sleep_for(250);
    //L
    led33 = led23 = led13 = led12 = led11 = 1;
    ThisThread::sleep_for(250);
    led33 = led23 = led13 = led12 = led11 = 0;
    ThisThread::sleep_for(250);
    //O
    led11 = led12 = led13 = led23 = led33 = led32 = led31 = led21 = 1;
    ThisThread::sleep_for(250);
    led11 = led12 = led13 = led23 = led33 = led32 = led31 = led21 = 0;
    ThisThread::sleep_for(250);
    }
}


//---------------------------- Temperature Function (STTS751 temp) ----------------------------

void Temper()
{
    leds();
    while(true)
	{
        temp->get_temperature(&value1);
        printf("%f \n", value1);
        // convert = (value1 * 1.8) + 32;

        if ((value1 >= 0) && (value1 <= 28))
        {
            //code for LED voltage control (via PWM maybe?)
            //alternative : just turn on 1st column of LEDs
            turn_off();
            temp->get_temperature(&value1);
        }

        if ((value1 > 28) && (value1 <= 38))
        {
            //Turn on 1st column of LEDs
            led11 = led21 = led31 = 1;
            led12 = led22 = led32 = 0;
            temp->get_temperature(&value1);
        }

        if ((value1 > 38) && (value1 <= 42))
        {
            //Turn on first 2 columns of LEDs
            led11 = led21 = led31 = 1;
            led12 = led22 = led32 = 1;
            led13 = led23 = led33 = 0;
            temp->get_temperature(&value1);
        }

        if ((value1 > 42) && (value1 <= 45))
        {
            //Turn on all 3 columns of LEDs
            turn_on();
            temp->get_temperature(&value1);
        }

        while ((value1 > 45))
        {
            WarningBlink();
            temp->get_temperature(&value1);
            wait_us(blink*1000);
        }

        wait_us(500000);
    }

    //Adjust input to LEDs based on convert
    //LEDs should be dim at some point and brighten and shift as temp increases

    /* For example : 
    from 32F to 50F ( 32 <= convert <= 50 )
    11
    21  Light up but at the dimmest at a sensed temperature from 32F, brighten up until max brightness @ 50F 
    31  and stay on if temp is in the next range
        from 50F to 68F ( 50 <= convert <= 68 )
        12
        22  Light up -||-
        32
            from 68F to 86F ( 68 <= convert <= 86 )
            13
            23 Light up -||-
            33
    Based on convert value, adjust voltage to relevant LEDs accordingly (linear function)
    */
}

//---------------------------- Humidity Function (HTS221 humid & temp) ----------------------------

void Humid()
{
    leds(); 
    
    //printf("entering the function");
    while(true)
	{
		hum_temp->get_humidity(&value2);
		printf("[hum] %.2f%%\r\n", value2);

		if ((value2 >= 0) && (value2 <= 25)) 
		{
			turn_off();
			hum_temp->get_humidity(&value2);
		}

		if ((value2 > 25) && (value2 <= 30)) 
		{
			led11 = led21 = led31 = 1;
			led12 = led22 = led32 = 0;
			led13 = led23 = led33 = 0;
			hum_temp->get_humidity(&value2);
		}

		if ((value2 > 30) && (value2 <= 60)) 
		{
			led11 = led21 = led31 = 1;
			led12 = led22 = led32 = 1;
			led13 = led23 = led33 = 0;
			hum_temp->get_humidity(&value2);
		}

		if ((value2 > 60) && (value2 < 70)) 
		{
			led11 = led21 = led31 = 1;
			led12 = led22 = led32 = 1;
			led13 = led23 = led33 = 1;
			hum_temp->get_humidity(&value2);
		}
		while ((value2 >= 70))
		{
			WarningBlink();
			hum_temp->get_humidity(&value2);
			wait_us(blink*1000);
		}
		wait_us(500000);
    }
}

//Define ISRs for the interrupts
void FLAG1()
{
	flag = 1;
}

void FLAG2()
{
	flag = 2;
}

void FLAG3()
{
	flag = 3;
}

void FLAG4()
{
    printf("flag4");
	flag = 4;
}

void trigger()
{
    counter ++;
}

int main()
{   
    
    printf("0");
    char buffer1[32], buffer2[32], buffer3[32];
    int32_t axes[3];

    // Enable sensors
    hum_temp->enable(); // HUMIDITY & TEMPERATURE SENSOR
    press_temp->enable(); // PRESSURE SENSOR
    temp->enable(); // TEMPERATURE SENSOR
    magnetometer->enable(); // MAGNETIC SENSOR
    accelerometer->enable_x(); // ANGLE ALONE (roll and pitch)
    acc_gyro->enable_x(); // ANGLE IN GYRO (roll and pitch)
    acc_gyro->enable_g(); // POSITIONS IN GYRO

    ledR = 1;

    __enable_irq();

    user_press.rise(&trigger);

	// Display modes selectable
    printf("> Press user button to start the function :\n");
    printf("\n| 1 time  = MoveInCube()\n");
    printf("| 2 times = Angle()\n");
    printf("| 3 times = Temper()\n");
    printf("| 4 times = Humid()\n");
    ThisThread::sleep_for(10000); //10s allowed for mode selection

    switch(counter)
	{
        case 1:
            MoveInCube();
        case 2:
            Angle();
        case 3:
            Temper();
        case 4:
            Humid();        
    }

}
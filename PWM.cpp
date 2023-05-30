/*
-----------------------------------------------------------------------------------
Analog Input and PWM
-----------------------------------------------------------------------------------
	Use two potentiometers to adjust the volume and pitch of the output sound wave.
	
	Inputs: potentiometers 1 and 2
	Output: speaker, PC
-----------------------------------------------------------------------------------
*/

#include "mbed.h"

//Define analog inputs
AnalogIn volPot(A0);
AnalogIn freqPot(A1);

//Define the PWM speaker output
PwmOut piezo(D6);

//Define variables
float i;
float volume;
float pitch;

int main()
{
	while(1)
    {
		/*
		Create a saw-tooth sound wave
		Make the period and volume adjustable via potentiometers
		*/
        volume = volPot.read();
        pitch = freqPot.read();

        // Equation for pitch control
        piezo.period(0.003125-pitch*0.003);

        for(i=0.0; i<1.0; i+=0.025)
        {
            piezo = volume * i;    
            ThisThread::sleep_for(1ms);
        }
	}
}
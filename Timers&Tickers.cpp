// Program: Timers & Tickers

#include "mbed.h"
#define SPEAKER D3

// Define the frequency of basic music notes
# define Do 0.5
# define Re 0.45
# define Mi 0.4
# define Fa 0.36
# define So 0.33
# define La 0.31
# define Si 0.3
# define No 0

// Define the beat length (e.g. whole note, half note)
# define b1 0.5
# define b2 0.25
# define b3 0.125
# define b4 0.075

// Define the musical piece
float note[] = {Mi,No,Mi,No,Mi,No, Mi,No,Mi,No,Mi,No,
Mi,No,So,No,Do,No,Re,No,Mi,No, Fa,No,Fa,No,Fa,No,Fa,No,
Fa,No,Mi,No,Mi,No,Mi,No,Mi,No, Mi,Re,No,Re,Mi, Re,No,So,No};

float beat[] = {b3,b3,b3,b3,b2,b2, b3,b3,b3,b3,b2,b2,
b3,b3,b3,b3,b3,b3,b3,b3,b2,b1, b3,b3,b3,b3,b3,b3,b3,b3,
b3,b3,b3,b3,b3,b3,b4,b4,b4,b4, b2,b3,b3,b2,b2, b2,b2,b2,b2};

// Define the PWM output for the speaker
PwmOut speaker(D3);
// Define the time ticker
Ticker timer;
// Static variable
static int k;
int noteSize = sizeof(note)/sizeof(note[0]);
2

// Define the ticker ISR
void timer_ISR()
{
	/*
	The time ticker ISR will be periodically triggered after every
	single note
	+ Update the PWM frequency to the next music note
	+ Update beat length for the next music note (reconfigure the tick interrupt time)
	*/
	if(k < noteSize)
	{
		if(note[k] == 0)
		{
			speaker = 0;
		}
		else
		{
			speaker = 0.1;
			speaker.period(0.002 * note[k]);
		}
		k++;
		timer.attach(&timer_ISR, beat[k]);
	}
	else
	{
		k = 0;
	}
}

int main()
{
	// Initialize the time ticker
	timer.attach(&timer_ISR, beat[k]);
	while(1)
	{
		// Sleep and wait for interrupts
		ThisThread::sleep_for(100);
	}
}
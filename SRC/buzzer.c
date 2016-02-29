#include "2440addr.h"
#include "stdio.h"
#include "uart.h"
#include <math.h>
#include "buzzer.h"
#include "libc.h"

static int buzzer_hz = 1; 				
static int buzzer_is_on = 1;	
static int buzzer_hz_init = 0;			
static int buzzer_hz_range = 1000;		
static int buzzer_period = 100;		


void buzzer_init(void)
{
	/* 
	* TO DO : Timer0 Init 
	* Prescaler value : 255, dead zone length = 0
	* Divider value : 1/16, no DMA mode
	* New frequency : (PCLK/(Prescaler value+1))*Divider value = (50Mhz/(255))*(1/16)
	*				= 12207 hz
	*/
	rTCFG0 = (0xff<<8)|(0); 
	rTCFG1 = (0<<20)|(3<<12); 
	
	/* TCON¼³Á¤ :Dead zone disable,  auto reload on, output inverter off
	*  manual update no operation, timer0 stop, TCNTB0=0, TCMPB0 =0
	*/
	rTCON  = (0<<20)|(1<<19)|(1<<18)|(0<<17)|(0<<16);
	rTCNTB3 = 0;
	rTCMPB3 = 0;
  
}
void buzzer_on(void)
{
	//cnt setting
   rTCNTB3 = 12207/buzzer_hz;
   rTCON |= (1<<17)|(0<<16);
   rTCON &= ~(1<<17);
   rTCON |= (1<<18)|(1<<16);
}
 
void buzzer_off(void)
{
   rTCNTB3 = 0;
   rTCON &= ~(1<<16);
}
 
void buzzer_set_frequency(int hz)
{
   buzzer_hz = hz;
   if (buzzer_is_on){
     buzzer_on();
   }
}
 
void buzzer_sine(int period)
{
   int i;
   buzzer_period = period;
   for(i = 0; i < buzzer_period; i++)
   {
     buzzer_hz = buzzer_hz_init + buzzer_hz_range * sin(buzzer_period * PI/180);
   }
   if (buzzer_is_on)
   {
         buzzer_on();
   }
}

void buzzer_play(int freq, int length)
{
	int time=0;
	
	while(1)
	{
		buzzer_set_frequency(freq);
		buzzer_sine(10);	
		
		if(time>=length)
		{
			buzzer_off();
			break;
		}
		
		Timer_Delay(1);
		time += 1;
	}
	
}

void buzzer_delay(int length)
{
	buzzer_off();
	Timer_Delay(length);	
}

#include "2440addr.h"


void Touch_Isr_Init(void (*fp)(void));
void Touch_ISR_Enable(int enable);
// Functions Declaration
char *wave_dat = (char *)0x31000100;
int sample = 1000;
void HW_Initial(void);

// Global Variables Declaration

volatile int ADC_x=0, ADC_y=0;		// ADC °ª
extern volatile int Touch_pressed;
extern volatile int Touch_x, Touch_y;		
volatile int Time = 0;


void Touch_Isr_Init(void (*fp)(void))
{
	rADCDLY=(50000);	
	// Enable Prescaler,Prescaler,AIN5/7 fix (MUX don't care),Normal,Disable read start,No operation
	rADCCON = (1<<14)+(39<<6)+(0<<3)+(0<<2)+(0<<1)+(0);	
	// Wait Down,YM:GND,YP:Hi-z,XM:Hi-z,XP:DIS,XP pullup En,Normal,Waiting for interrupt mode     
	rADCTSC=(0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);

	pISR_ADC = (unsigned int)fp;
}

void Touch_ISR_Enable(int enable)
{	
	enable? (rINTMSK &= (unsigned)(~(BIT_ADC))) : (rINTMSK |= (unsigned)(BIT_ADC));
	enable? (rINTSUBMSK &= (unsigned)(~(BIT_SUB_TC))) : (rINTSUBMSK |= (unsigned)(BIT_SUB_TC));	
}


__irq void Touch_ISR(void)
{
	unsigned int temp = rINTMSK;
	rINTMSK  = BIT_ALLMSK;	

	rINTSUBMSK|=(BIT_SUB_ADC|BIT_SUB_TC);

	// Touch UP
	if(rADCTSC&0x100)
	{
		rADCTSC&=0xff;
		Touch_pressed = 0;
	}
	// Touch Down
	else 
	{
		rADCTSC=(0<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(1<<3)|(1<<2)|(0);
		// SADC_ylus Down,Don't care,Don't care,Don't care,Don't care,XP pullup Dis,Auto,No operation
		rADCCON|=0x1;
		while(rADCCON & 0x1);
		while(!(0x8000&rADCCON));
		ADC_x=(int)(0x3ff&rADCDAT0); //x¿Í y¸¦ ¹Ù²Þ
		ADC_y=(int)(0x3ff&rADCDAT1);

		Touch_y=(ADC_y-80)*(272-10)/(900-80)+5;
		Touch_x=(ADC_x-940)*(480-10)/(70-940)+5;
		Touch_x=480-Touch_x;
		if(Touch_x<0) Touch_x=0;
		if(Touch_x>=480) Touch_x=480-1;
		if(Touch_y<0) Touch_y=0;
		if(Touch_y>=272) Touch_y=272-1;

		rADCTSC=(1<<8)|(1<<7)|(1<<6)|(0<<5)|(1<<4)|(0<<3)|(0<<2)|(3);
		// SADC_ylus Up,Don't care,Don't care,Don't care,Don't care,XP pullup En,Normal,Waiting mode
		Touch_pressed = 1; 
	}
	rSUBSRCPND|=BIT_SUB_TC;
	rINTSUBMSK = (unsigned)(~(BIT_SUB_TC));
	rSRCPND = (unsigned)BIT_ADC;
	rINTPND = (unsigned)BIT_ADC;  

	rINTMSK = temp;
}
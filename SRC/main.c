#include <stdarg.h>
#include "2440addr.h"
#include "buzzer.h"

__irq void Touch_ISR(void);
volatile int Touch_pressed = 0;		// 터치패드가 눌림을 알수 있는 값
volatile int Touch_x, Touch_y;		// Calibration 정보 저장 값


void All_Init();


void Main()
{
	int gameMode;
	int i=0;
	int mario[]=
	{
		_MI,NONE, _MI,NONE, _MI,NONE, _DOH, _MI, _SOL, SOL,
		
		_DOH, SOL, MI, RA, SI, SI_P, RA,
		SOL, _MI, _SOL, _RA, _PA, _SOL, _MI, _DOH, _LAE, SI,
		_DOH, SOL, MI, RA, SI, SI_P, RA,
		SOL, _MI, _SOL, _RA, _PA, _SOL, _MI, _DOH, _LAE, SI,
		
		_SOL, _PA_S, _PA, _LAE_P, _MI, SOL, RA, _DOH, RA, _DOH, _LAE,
		_SOL, _PA_S, _PA, _LAE_P, _MI, _DOH, _DOH, _DOH,
		_SOL, _PA_S, _PA, _LAE_P, _MI, SOL, RA, _DOH, RA, _DOH, _LAE,
		_SOL, _PA_S, _PA, _LAE_P, _MI, _DOH, _DOH, _DOH,
		_LAE_S, _LAE, _DOH,
		
		_DOH, _DOH, _DOH, _DOH, _LAE, _MI, _DOH, RA, SOL,
		_DOH, _DOH, _DOH, _DOH, _LAE, _MI,
		_DOH, _DOH, _DOH, _DOH, _LAE, _MI, _DOH, RA, SOL,
		_DOH, _DOH, _DOH, _DOH, _LAE, _MI,
		_MI, _MI, _MI, _DOH, _MI, _SOL, SOL,
		
		_DOH, SOL, MI, RA, SI, SI_P, RA,
		SOL, _MI, _SOL, _RA, _PA, _SOL, _MI, _DOH, _LAE, SI,
		
		_MI, _DOH, SOL, SOL_S, RA, _PA, _PA, RA,
		SI, _RA, _RA, _SOL, _PA, _MI, _DOH, RA, SOL,
		_MI, _DOH, SOL, SOL_S, RA, _PA, _PA, RA,
		SI, _RA, _RA, _SOL, _PA, _DOH, _MI, _MI, _DOH,
		_MI, _DOH, SOL, SOL_S, RA, _PA, _PA, RA,
		SI, _RA, _RA, _SOL, _PA, _MI, _DOH, RA, SOL,
		_MI, _DOH, SOL, SOL_S, RA, _PA, _PA, RA,
		SI, _RA, _RA, _SOL, _PA, _DOH, _MI, _MI, _DOH
		
	};
	int delay[]=
	{
		250,10, 200,10, 450,50, 250, 300, 400, 500,
		
		400, 400, 400, 200, 200, 100, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,
		
		400, 400, 400, 200, 200, 100, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,
		
		400, 400, 400, 200, 200, 100, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,
		
		400, 400, 400, 200, 200, 100, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,
		
		400, 400, 400, 200, 200, 100, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,

		400, 400, 400, 200, 200, 100, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200,
		200, 200, 200, 200, 200, 200, 200
				
	};
	
	//
	pISR_ADC = (unsigned int)Touch_ISR;
	rINTSUBMSK &= ~(0x1<<9);
	rINTMSK &= ~(0x1<<31);	
	Touch_Isr_Init(Touch_ISR);
	//
	
			
/*
	while(1)
	{
		if(Touch_pressed==1)
		{
			Lcd_Clr_Screen(WHITE);
		}
	}
*/	
			
			
   	Uart_Printf("*************\n");
   	Uart_Printf("* Game Boy  *\n");
   	Uart_Printf("*************\n");
   	Uart_Printf("*************\n");
   	Uart_Printf("*Select Game*\n");
   	Uart_Printf("*************\n");
   	
   	All_Init();
   	
   	gameMode = start_pgm(mario,delay);   
   	
   	while(1)
   	{
   		Uart_Printf("1 : gugudan\n 2 : abspitch\n 3 : baseball\n");
             
      	switch(gameMode)
      	{	
         	case 1:
         		Uart_Printf("music_game\n");
            	start_musicGame();
            	break;
         	case 2:
         		Uart_Printf("gugudan");
	            start_gugudan();
	            break;
	        case 3:
	        	Uart_Printf("baseball\n");
	            start_baseball();
	            break;
	        default:
	            Uart_Printf("Input Game Number\n");
	            break;
	     }
      	 gameMode = menu_pgm();
    }
	
}


void All_Init()
{
	Led_Init();
	Key_Port_Init();
	buzzer_init();
	Graphic_Init();
	
}

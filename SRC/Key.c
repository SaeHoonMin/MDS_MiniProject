#include "2440addr.h"
#include "uart.h"


void Key_Port_Init(void)
{
	// KEYOUT[0:4] = GPB[0:2] & GPH[9] & GPB[4], KEYIN[0:1] = GPF[3:4]
	
	/* TODO : GPFCON - Input mode */
	rGPFCON &= ~(0xf<<6);
	rGPFUP &= ~(0x3<<3);
	
	/* TODO : GPBCON & GPHCON - output mode */
	
	rGPBCON &= ~0x3F;
	rGPBCON |= 0x15;
	
	rGPHCON &= ~(0x3<<18);
	rGPHCON |= 0x1<<18;
	rGPBCON &= ~(0x3<<8);
	rGPBCON |= 0x1<<8;
}

int Key_Get_Pressed()
{
	int i,retval = 0;	

	for(i=0;i<5;i++)
	{

		switch(i)
		{
			case 0:
			rGPBDAT &= ~(0x01<<i);
			retval = chkrGPFDAT();
			rGPBDAT |= 0x01<<i;
			if(retval)
			{
				return retval + 2*i;
			}
			break;

			case 1:
			rGPBDAT &= ~(0x01<<i);
			retval = chkrGPFDAT();
			rGPBDAT |= 0x01<<i;
			if(retval)
			{
				return retval + 2*i;
			}
			break;
			
			case 2:
			rGPBDAT &= ~(0x01<<i);
			retval = chkrGPFDAT();
			rGPBDAT |= 0x01<<i;
			if(retval)
			{
				return retval + 2*i;
			}
			break;
			
			case 3:
			rGPHDAT &= ~(0x01<<9);
			retval = chkrGPFDAT();
			rGPHDAT |= 0x01<<9;
			if(retval)
			{
				return retval + 2*i;
			}
			break;
		
			case 4:
			rGPBDAT &= ~(0x01<<i);
			retval = chkrGPFDAT();
			rGPBDAT |= 0x01<<i;
			if(retval)
			{
				return retval + 2*i;
			}
			break;
			
			default:
				break;
		}
	}
	return 0;
}

int chkrGPFDAT()
{
	if(rGPFDAT == 0x18)
		return 0;
	else if( rGPFDAT == 0x08)
		return 3;
	else if( rGPFDAT == 0x10)
		return 2;
}

int Key_Wait_Get_Pressed()
{
	/* 키가 눌려졌을 때에만 값을 리턴하도록 기다려 주는 함수 */
	while( Key_Get_Pressed() == 0 )
	{
	}
	return a;
	
}

void Key_Wait_Get_Released()
{	
	/*  키가 눌려지지 않았을 때를 기다리는  함수 */
	while(  Key_Get_Pressed() )
	{
	}
}
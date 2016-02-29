/*
 * =====================================================================
 * NAME         : Led.c
 *
 * Descriptions : Main routine for S3C2440
 *
 * IDE          : CodeWarrior 2.0 or Later
 *
 * Modification
 *	  
 * =====================================================================
 */

#include "2440addr.h"
#include "libc.h"

//Function Declaration
void Led_Init();
void Led_On(int num);
void Led_Off(int num);

void Led_Init()
{
	int i;
	
	// TO DO : Init GPIO port connected to LED 
	// LED[4:9] = GPB[5:10] 
	rGPBDAT &= ~(0x3f<<5);
	rGPBDAT &= ~(0x1<<3);
	rGPBCON &= ~(0xfff<<10);
	rGPBCON |= (0x555<<10);
	rGPBCON &= ~(0x3<<6);
	rGPBCON |= (0x1<<7);
	
	for(i = 0; i < 10; i++)
	{
		rGPBCON = rGPBCON & ~(1 << 10+2*i+1);
		rGPBCON = rGPBCON | (1 << 10+2*i);
		rGPBDAT = rGPBDAT & (1 << 5+i);
	}
		
}

void Led_On(int num)
{
	rGPBDAT = rGPBDAT | (1 << num+4);
}

void Led_Off(int num)
{
	rGPBDAT = rGPBDAT & ~(1 << num+4);
}


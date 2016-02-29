/*
 * =====================================================================
 * NAME         : Uart.c
 *
 * Descriptions : Uart Test Driver
 *
 * IDE          : CodeWarrior 2.0 or Later
 *
 * Modification
 *	   
 * =====================================================================
 */
#include "2440addr.h"
#include <stdarg.h>
#include "Uart.h"

// Function Declaration

/*******************************************************************************
* Function Name  : Uart_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_Init(int baud)
{
	// PORT GPIO initial
	rGPHCON &= ~(0xf<<4);
	rGPHCON |= (0xa<<4);	

    rGPGCON &= ~(0x3<<22);
    rGPGCON |= 0x2<<22;
    rGPGDAT |= 0x1<<11;
    rGPGUP |= 0x1<<11;
   // rGPGUP
    /*Uart tx02*/
	rGPHCON &= ~(0xf<<12);
	rGPHCON |= (0xa<<12); 
	/*  Non-FiFo모드 */
	rUFCON0 = 0x0; 
	rUMCON0 = 0x0;

	rUFCON2 = 0x0; 
	rUMCON2 = 0x0;
	/* Line Control(Normal mode, No parity, One stop bit, 8bit Word length */
	rULCON0 = 0x3;
	/* Uart2 Line Control(IRDA Mode, No parity, One stop bit, 8bit Word length */
	rULCON2 = 0;
	rULCON2	= 0x43;
	/* TO DO : Transmit & Receive Mode is interrupt request or polling mode  */
    rUCON0 &= ~0xf;
    rUCON0 |= 0x05;
	rUCON2 &= ~0xf;
	rUCON2 |= 0x05;

	/* baud rate 설정  */		
	rUBRDIV0= ((unsigned int)(PCLK/16./115200+0.5)-1 );
	rUBRDIV2= ((unsigned int)(PCLK/16./115200+0.5)-1 );
}

/*******************************************************************************
* Function Name  : Uart_Printf
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_Send_String(string);
    va_end(ap);		
}
/*******************************************************************************
* Function Name  : Uart_Send_String
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_Send_String(char *pt)
{
	while(*pt)
	{
		Uart_Send_Byte(*pt++);		
	}	
}
/*******************************************************************************
* Function Name  : Uart_Send_Byte
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_Send_Byte(int data)
{
	if(data=='\n')
	{
		while(!(rUTRSTAT0 & 0x2));
		WrUTXH0('\r');
	}

	while(!(rUTRSTAT0 & 0x2));	
		WrUTXH0(data);
}
/*******************************************************************************
* Function Name  : Uart_Send_Irda
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_Send_Irda(int data)
{
	if(data=='\n')
	{
		while(!(rUTRSTAT2 & 0x2));
		WrUTXH2('\r');
	}
	
	while(!(rUTRSTAT2 & 0x2));	
		WrUTXH2(data);
}

/*******************************************************************************
* Function Name  : Uart_Get_Char
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
char Uart_Get_Char()
{
	while(!(rUTRSTAT0 & 0x1));
	return RdURXH0();	
	//return *(volatile unsigned char *)URXH0;
	 
}
/*******************************************************************************
* Function Name  : Uart_Check_Overrun_Error
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int Uart_Check_Overrun_Error()
{
	/* overrun error check in ERROR Status Register  */
	while(!(rUERSTAT0 & 0x1));	
	return 1;

}

/*******************************************************************************
* Function Name  : Uart_RX0_ISR_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_RX0_ISR_Init(void (*fp)(void))
{
    rINTMOD &= ~BIT_UART0;
    

    pISR_UART0 = (unsigned int)fp;    
}
/*******************************************************************************
* Function Name  : Uart_RX0_ISREnalble
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_RX0_ISREnalble(int enable)
{
    enable?(rINTMSK &=(unsigned)(~BIT_UART0)):(rINTMSK |=(unsigned)BIT_UART0);

}
/*******************************************************************************
* Function Name  : Uart_RX0_ISR_SubEnalble
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_RX0_ISR_SubEnalble(int enable)
{
    enable?(rINTSUBMSK &=(unsigned)(~BIT_SUB_RXD0)):(rINTSUBMSK |=(unsigned)BIT_SUB_RXD0);

}
/*******************************************************************************
* Function Name  : Uart_RX0_ISR_SrcPnd
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_RX0_ISR_SrcPnd(int enable)
{
    enable?(rSRCPND |=(unsigned)(BIT_UART0)):(rSRCPND &=(unsigned)~BIT_UART0);

}
/*******************************************************************************
* Function Name  : Uart_RX0_ISR_IntPnd
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_RX0_ISR_IntPnd(int enable)
{
    enable?(rINTPND |=(unsigned)(BIT_UART0)):(rINTPND &=(unsigned)~BIT_UART0);

}
/*******************************************************************************
* Function Name  : Uart_RX0_ISR_SubSrcPnd
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Uart_RX0_ISR_SubSrcPnd(int enable)
{
    enable?(rSUBSRCPND |=(unsigned)(BIT_SUB_RXD0)):(rSUBSRCPND &=(unsigned)~BIT_SUB_RXD0);

}



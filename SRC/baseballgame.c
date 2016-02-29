#include "2440addr.h"
#include "uart.h"
#include <stdlib.h>
#include "buzzer.h"
#include "baseball.h"

#include ".\LCD\baseball_main.c"
#include ".\LCD\baseball_clear.c"
#include ".\LCD\baseball_noclear.c"
#include ".\LCD\baseball_ing.c"

//#define BASEBALL_TEST					//�׽�Ʈ �غ��� ���� define ��

int Ball_Num[4] = {1, 2, 3, 4};			//������ ����� �迭����
int Input_Num[4] = {0};					//����ڰ� �Է��� �迭����
int ball_cnt = 0;						//�� ī��Ʈ
int strike_cnt = 0;						//��Ʈ����ũ ī��Ʈ
int out_cnt = 0;						//�ƿ� ī��Ʈ	

static unsigned int ballSeed =0;

/*main function of baseball Game.*/

void start_baseball(void)				
{

	int i, j;
	int a;
	int temp;
	int k = 0;
	int sound_clr = 0;
		
	int baseball_sound[] = 
	{
	DOH, MI, SOL, DOH, MI, SOL, RA, RA, RA, SOL, PA, PA, PA, MI, MI, MI,
	LAE, LAE, LAE, SOL
	};

	int sound_delay[] = 
	{
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200,
	200, 200, 200, 200
	};
	
	Key_Port_Init();
	
	Uart_Printf("Press any key to start baseball game\n");
	Lcd_Draw_BMP(0, 0, baseball_main);
		
	while(1)
	{	
				
		if(baseball_sound[k]!=0)
		{
			buzzer_play(baseball_sound[k],sound_delay[k]);
		
		}
		else
			buzzer_delay(sound_delay[k]);
			
			
		sound_clr = Key_Num_Allocation();
			
		if(sound_clr != 0)
		{
			
			break;
		}
		
		k++;
	}
	
	while(1)
	{
		#ifndef BASEBALL_TEST
		ballSeed++;
		#endif
	
		if(Wait_Num() != 0)
			break;
	}
	
	#ifndef BASEBALL_TEST
	
	
	for(i=0;i<4;i++)
	{
		temp = (rand()+ballSeed)%10;
		
		for(j=0;j<i;j++)
		{
			if(Ball_Num[j] == temp)
			{
				i--;
				break;
			}
			
		}
			
		if(j == i)
			Ball_Num[i] = temp;
	}
	
	#endif
	
	for(i=0;i<4;i++)
	{
		Uart_Printf("%d\n", Ball_Num[i]);
	}
		
	Uart_Printf("Let the game begin\n");
	while(1)	
	{
	
		Lcd_Draw_BMP(0, 0, baseball_ing);	
		Input_Nums(Input_Num);
		
		for(i=0;i<4;i++)
		{
			
			Uart_Printf("Input_Num:%d\n", Input_Num[i]);
			
		}
		
		if((Input_Num[0] == 0) && (Input_Num[1] ==0) && (Input_Num[2] == 0) && (Input_Num[3] ==0))		//����� �Է��� 0000 �Է��� ��� ���� ��������
			break;
		
		for(j=0;j<0x1ffffff;j++);
		
		Compared_Num(Input_Num,Ball_Num,&strike_cnt,&ball_cnt,&out_cnt);
		
		Uart_Printf("strike_cnt:%d\n", strike_cnt);
		Uart_Printf("ball_cnt:%d\n", ball_cnt);
		
		Output_strike_Result(&strike_cnt);
		
		for(i=0;i<0x1ffffff;i++);
		
		Output_Ball_Result(&ball_cnt);
		
		if(strike_cnt == 4)							//4���� ���� ���߸� ���� �޽��� ����ϰ� ��������
		{
						
			break;
		}
		strike_cnt = 0;
		ball_cnt = 0;
		out_cnt = 0;
		
	}

	
}

                                
int Key_Num_Allocation()					//�� Ű�� ���� �Ҵ��ϰ� ��ȯ�ϴ� �Լ�
{
	
	int input = 0;
	int i, j;
	
	for(i=0;i<=10;i++)
	{
		
		switch(i)
		{
			case 0:
				
				rGPBDAT = rGPBDAT &~(0x1<<0);
				if(((rGPFDAT) & (0x1<<3)) == 0)
				{
					input = 2;
					rGPBDAT = rGPBDAT | (0x1<<0);
					return input;
				}
				rGPBDAT = rGPBDAT |(0x1<<0);
				break;
			
			case 1:
			
				rGPBDAT = rGPBDAT &~(0x1<<0);
				if(((rGPFDAT) & (0x1<<4)) == 0)
				{
					input = 3;
					rGPBDAT = rGPBDAT |(0x1<<0);
					return input;
				}
				rGPBDAT = rGPBDAT |(0x1<<0);
				break;
				
			case 2:
			
				rGPBDAT = rGPBDAT &~(0x1<<1);
				if(((rGPFDAT) & (0x1<<3)) == 0)
				{
					input = 4;
					rGPBDAT = rGPBDAT |(0x1<<1);
					return input;
				}
				rGPBDAT = rGPBDAT |(0x1<<1);
				break;
				
			case 3:
			
				rGPBDAT = rGPBDAT &~(0x1<<1);
				if(((rGPFDAT) & (0x1<<4)) == 0)
				{
					input = 5;
					rGPBDAT = rGPBDAT |(0x1<<1);
					return input;
				}
				rGPBDAT = rGPBDAT |(0x1<<1);
				break;
				
			case 4:
			
				rGPBDAT = rGPBDAT &~(0x1<<2);
				if(((rGPFDAT) &(0x1<<3)) == 0)
				{
					input = 6;
					rGPBDAT = rGPBDAT |(0x1<<2);
					return input;
				}
				rGPBDAT = rGPBDAT |(0x1<<2);
				break;
				
			case 5:
			
				rGPBDAT = rGPBDAT &~(0x1<<2);
				if(((rGPFDAT) & (0x1<<4)) == 0)
				{
					input = 7;
					rGPBDAT = rGPBDAT |(0x1<<2);
					return input;
				}
				rGPBDAT = rGPBDAT |(0x1<<2);
				break;	
				
			case 6:
			
				rGPHDAT = rGPHDAT &~(0x1<<9);
				if(((rGPFDAT) &(0x1<<3)) == 0)
				{
					input = 8;
					rGPHDAT = rGPHDAT |(0x1<<9);
					return input;
				}
				rGPHDAT = rGPHDAT |(0x1<<9);
				break;
				
			case 7:
			
				rGPHDAT = rGPHDAT &~(0x1<<9);
				if(((rGPFDAT) & (0x1<<4)) == 0)
				{
					input = 9;
					rGPHDAT = rGPHDAT |(0x1<<9);
					return input;
				}
				rGPHDAT = rGPHDAT |(0x1<<9);
				break;	
				
			case 8:
			
				rGPBDAT = rGPBDAT &~(0x1<<4);
				if(((rGPFDAT) &(0x1<<3)) == 0)
				{
					input = 10;
					rGPBDAT = rGPBDAT |(0x1<<4);
					return input;
				}
				rGPBDAT = rGPBDAT |(0x1<<4);
				break;
				
			case 9:
			
				rGPBDAT = rGPBDAT &~(0x1<<4);
				if(((rGPFDAT) & (0x1<<4)) == 0)
				{
					input = 11;
					rGPBDAT = rGPBDAT |(0x1<<4);
					return input;
				}
				rGPBDAT = rGPBDAT |(0x1<<4);
				break;
					
			default:
			
				return 0;			
	
		}
			
	}
		
		
}


int Wait_Num()									//�Է� ���� �� ��� 
{
	int Input = 0;
	
	while(Input == 0)
	{
		Input = Key_Num_Allocation();
	}
	
	return Input;
}


void Input_Nums(int *Input_Num)					//�Է� ���� ���� �迭�� ���� �Լ�	
{
	
	int i = 0, j;
	int input;
		
	while(1)
	{
		input = Wait_Num();
		for(j=0;j<0x1ffffff;j++);
		Input_Num[i] = input - 2;
	
		i++;
		
		if(i == 4)
			break;
		
	}
			
}   

 

void Compared_Num(int *Input_Num, int *Ball_Num, int *scnt, int *bcnt, int *ocnt)			//���� ��ȣ�� ����� �Է� ��ȣ���� �� �Լ�
{
	int i, j;
	int cnt = 0;					//�� �ڸ��� ���� �ٸ��� �ľ��ϱ� ���� ī����
	

	for(i=0;i<4;i++)
	{
		
		for(j=0;j<4;j++)
		{	
			
			if(Input_Num[i] == Ball_Num[j])
			{	
				
				if(i == j)
				{
					(*scnt)++;
					
					
									
				}
				else if(i != j)
				{
					(*bcnt)++;
					
					
				}
			}else
		
				cnt++;
		}
	
	}
	
	if(cnt == 16)
	{
		(*ocnt)++;
		
	}
		
}
	
		
void Output_strike_Result(int *scnt)						//��Ʈ����ũ ����� ���� LED ���
{
	
	int i;
	
	if(*scnt == 1)
	{	
		Lcd_Draw_BMP(0, 0, baseball_noclear);
		rGPBDAT = rGPBDAT | (0x1<<5);
		for(i=0;i<0xfffffff;i++);
		rGPBDAT = rGPBDAT &~(0x1<<5);
	}
	else if(*scnt == 2)
	{
		Lcd_Draw_BMP(0, 0, baseball_noclear);
		rGPBDAT = rGPBDAT | (0x1<<5);
		rGPBDAT = rGPBDAT | (0x1<<6);
		for(i=0;i<0xfffffff;i++);
		rGPBDAT = rGPBDAT &~(0x1<<5);
		rGPBDAT = rGPBDAT &~(0x1<<6);
	}	
	else if(*scnt == 3)
	{
		Lcd_Draw_BMP(0, 0, baseball_noclear);
		rGPBDAT = rGPBDAT | (0x1<<5);
		rGPBDAT = rGPBDAT | (0x1<<6);
		rGPBDAT = rGPBDAT | (0x1<<7);
		for(i=0;i<0xfffffff;i++);
		rGPBDAT = rGPBDAT &~(0x1<<5);
		rGPBDAT = rGPBDAT &~(0x1<<6);
		rGPBDAT = rGPBDAT &~(0x1<<7);
	}
	else if(*scnt == 4)
	{
		
		rGPBDAT = rGPBDAT | (0x1<<5);
		rGPBDAT = rGPBDAT | (0x1<<6);
		rGPBDAT = rGPBDAT | (0x1<<7);
		rGPBDAT = rGPBDAT | (0x1<<8);
		rGPBDAT = rGPBDAT | (0x1<<9);
		rGPBDAT = rGPBDAT | (0x1<<10);
		
		Lcd_Draw_BMP(0, 0, baseball_clear);
		Uart_Printf("Game Clear!!\n");
		
		for(i=0;i<0xfffffff;i++);
		rGPBDAT = rGPBDAT &~(0x1<<5);
		rGPBDAT = rGPBDAT &~(0x1<<6);
		rGPBDAT = rGPBDAT &~(0x1<<7);
		rGPBDAT = rGPBDAT &~(0x1<<8);
		rGPBDAT = rGPBDAT &~(0x1<<9);
		rGPBDAT = rGPBDAT &~(0x1<<10);
		
	}	
}
					
void Output_Ball_Result(int *bcnt)							//�� ����� ���� LED ���
{
	
	int i;
	
	if(*bcnt == 1)
	{	
		Lcd_Draw_BMP(0, 0, baseball_noclear);
		rGPBDAT = rGPBDAT | (0x1<<5);
		for(i=0;i<0xfffffff;i++);
		rGPBDAT = rGPBDAT &~(0x1<<5);
	}
	else if(*bcnt == 2)
	{
		Lcd_Draw_BMP(0, 0, baseball_noclear);
		rGPBDAT = rGPBDAT | (0x1<<5);
		rGPBDAT = rGPBDAT | (0x1<<6);
		for(i=0;i<0xfffffff;i++);
		rGPBDAT = rGPBDAT &~(0x1<<5);
		rGPBDAT = rGPBDAT &~(0x1<<6);
	}	
	else if(*bcnt == 3)
	{
		Lcd_Draw_BMP(0, 0, baseball_noclear);
		rGPBDAT = rGPBDAT | (0x1<<5);
		rGPBDAT = rGPBDAT | (0x1<<6);
		rGPBDAT = rGPBDAT | (0x1<<7);
		for(i=0;i<0xfffffff;i++);
		rGPBDAT = rGPBDAT &~(0x1<<5);
		rGPBDAT = rGPBDAT &~(0x1<<6);
		rGPBDAT = rGPBDAT &~(0x1<<7);
	}
	else if(*bcnt == 4)
	{
		Lcd_Draw_BMP(0, 0, baseball_noclear);
		rGPBDAT = rGPBDAT | (0x1<<5);
		rGPBDAT = rGPBDAT | (0x1<<6);
		rGPBDAT = rGPBDAT | (0x1<<7);
		rGPBDAT = rGPBDAT | (0x1<<8);
		for(i=0;i<0xfffffff;i++);
		rGPBDAT = rGPBDAT &~(0x1<<5);
		rGPBDAT = rGPBDAT &~(0x1<<6);
		rGPBDAT = rGPBDAT &~(0x1<<7);
		rGPBDAT = rGPBDAT &~(0x1<<8);
		
	}
}

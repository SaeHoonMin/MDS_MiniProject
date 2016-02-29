#include <stdio.h>
#include <stdarg.h>
#include ".\LCD\main_gugudan.c"
#include "buzzer.h"
#include ".\LCD\click.c"
#include ".\LCD\googookey.c"
#include ".\LCD\zero.c"
#include ".\LCD\one.c"
#include ".\LCD\two.c"
#include ".\LCD\three.c"
#include ".\LCD\four.c"
#include ".\LCD\five.c"
#include ".\LCD\six.c"
#include ".\LCD\seven.c"
#include ".\LCD\eight.c"
#include ".\LCD\nine.c"
#include ".\LCD\gob.c"
#include ".\LCD\noon.c"
#include ".\LCD\OK.c"
#include ".\LCD\NO.c"


extern volatile int Touch_pressed;
extern volatile int Touch_x, Touch_y;

int googoo (void);
int answer (void);
void print_comeon (void);  //
void goo_music(void);
unsigned int q=0;

int position[] = {1,46,50,94,99,141,147,190,195,237,243,285,291,333,339,381,387,429,435,478};
	

void start_gugudan()
{
   int save, ans;
   
   int goo_music[]= 
	{
		SOL,NONE, SOL,NONE, MI,NONE, LAE,NONE, SOL,NONE, SOL,NONE,
		SOL,NONE, SOL,NONE, MI,NONE, LAE,NONE, SOL,NONE, SOL,NONE,
		SOL,NONE, SOL,NONE, MI,NONE, LAE,NONE, SOL,NONE, SOL,NONE,
        SOL,NONE, SOL,NONE, MI,NONE, LAE,NONE, SOL,NONE, SOL,NONE,
        SOL,NONE, SOL,NONE, MI,NONE, LAE,NONE, SOL,NONE, SOL,NONE,
        SOL,NONE, SOL,NONE, MI,NONE, LAE,NONE, SOL,NONE, SOL,NONE,
        SOL,NONE, SOL,NONE, MI,NONE, LAE,NONE, SOL,NONE, SOL,NONE,
        SOL,NONE, SOL,NONE, MI,NONE, LAE,NONE, SOL,NONE, SOL,NONE,
        SOL,NONE, SOL,NONE, MI,NONE, LAE,NONE, SOL,NONE, SOL,NONE,
        SOL,NONE, SOL,NONE, MI,NONE, LAE,NONE, SOL,NONE, SOL,NONE
	};
	int goo_delay[]=
	{
		200,5,200,10,150,10,100,10,200,10,250,500,
        200,5,200,10,150,10,100,10,200,10,250,500,
        200,5,200,10,150,10,100,10,200,10,250,500,
        200,5,200,10,150,10,100,10,200,10,250,500,
        200,5,200,10,150,10,100,10,200,10,250,500,
        200,5,200,10,150,10,100,10,200,10,250,500,
        200,5,200,10,150,10,100,10,200,10,250,500,
        200,5,200,10,150,10,100,10,200,10,250,500,
        200,5,200,10,150,10,100,10,200,10,250,500,
        200,5,200,10,150,10,100,10,200,10,250,500
	};
    extern volatile int Touch_pressed;
   	int j = 0, a = 0, res;
   	Key_Port_Init();  //초기화 
   	Lcd_Draw_BMP(0, 0, main_gugudan);  //이미지파일
   	Lcd_Draw_RMBG_BMP(0, 0, click);   //이미지파일 
   	
   	while(1)
   	{
   		if(goo_music[j]!=0)
   		{
			buzzer_play(goo_music[j],goo_delay[j]);
   		}
   		else
	    {	   
			buzzer_delay(goo_delay[j]);
	    }
		j++;
		q++;
			if(Touch_pressed==1)
		{
			break;
		}
   	}
   	Lcd_Draw_BMP(0, 0, googookey);
 
   
   while(1)
   {
      save = googoo(); //랜덤 두개 곱한 값
      ans = answer();   //두개 입력

      if(ans == 0)
      {
         break;
      }
      Uart_Printf("\n");
      if( save == ans)
      {
         Uart_Printf("OK\n");
         Lcd_Draw_BMP(0, 0, OK);
         for(j=0;j<0x1ffffff;j++);
      }
      else
      {
         Uart_Printf("NO\n");
         Lcd_Draw_BMP(0, 0, NO);
         for(j=0;j<0x1ffffff;j++);
      }
   }
}

int googoo (void)
{
   int a,b,res,rand();
   a = ((rand()+q)%9)+1;
   b = ((rand()+q)%9)+1;
   res = a * b;
   Uart_Printf("%d x %d = ",a,b);
   change_num(a, 0, 1);
   Lcd_Draw_RMBG_BMP(80, 0, gob);
   change_num(b, 162, 0);
   Lcd_Draw_RMBG_BMP(244, 0, noon);
  	  
   		 
   return res;
}

int answer (void)
{
   int num[2];
   int key = -1;
   int count = 0;
   int check=0;
   while(1)
   {
      if(count == 2) break;
      
      //Key_Wait_Get_Released();
      //key = Key_Wait_Get_Pressed()-2;
      while(1)
      {
	      if(!Touch_pressed&&(Touch_x >=1) && (Touch_x <=46) && (Touch_y >= 141) && (Touch_y <= 271))
		  {
				Touch_x=0;
				Touch_y=0;
				check=!check;
				key=0;
				if(check==1)
				{
					change_num(key, 326, 0);
				}
				else
				{
					change_num(key, 408, 0);
				}
				break;
		  }
		  else if(!Touch_pressed&&(Touch_x >=50) && (Touch_x <=94) && (Touch_y >= 141) && (Touch_y <= 271))
		  {
				Touch_x=0;
				Touch_y=0;
				check=!check;
				key=1;
				if(check==1)
				{
					change_num(key, 326, 0);
				}
				else
				{
					change_num(key, 408, 0);
				}
				break;
		  }
		  else if(!Touch_pressed&&(Touch_x >=99) && (Touch_x <=141) && (Touch_y >= 141) && (Touch_y <= 271))
		  {
				Touch_x=0;
				Touch_y=0;
				check=!check;
				key=2;
				if(check==1)
				{
					change_num(key, 326, 0);
				}
				else
				{
					change_num(key, 408, 0);
				}
				break;
		  }
		  else if(!Touch_pressed&&(Touch_x >=147) && (Touch_x <=190) && (Touch_y >= 141) && (Touch_y <= 271))
		  {
				Touch_x=0;
				Touch_y=0;
				check=!check;
				key=3;
				if(check==1)
				{
					change_num(key, 326, 0);
				}
				else
				{
					change_num(key, 408, 0);
				}
				break;
		  }
		  else if(!Touch_pressed&&(Touch_x >=195) && (Touch_x <=237) && (Touch_y >= 141) && (Touch_y <= 271))
		  {
				Touch_x=0;
				Touch_y=0;
				check=!check;
				key=4;
				if(check==1)
				{
					change_num(key, 326, 0);
				}
				else
				{
					change_num(key, 408, 0);
				}
				break;
		  }
		  else if(!Touch_pressed&&(Touch_x >=243) && (Touch_x <=285) && (Touch_y >= 141) && (Touch_y <= 271))
		  {
				Touch_x=0;
				Touch_y=0;
				check=!check;
				key=5;
				if(check==1)
				{
					change_num(key, 326, 0);
				}
				else
				{
					change_num(key, 408, 0);
				}
				break;
		  }
		  else if(!Touch_pressed&&(Touch_x >=291) && (Touch_x <=333) && (Touch_y >= 141) && (Touch_y <= 271))
		  {
				Touch_x=0;
				Touch_y=0;
				check=!check;
				key=6;
				if(check==1)
				{
					change_num(key, 326, 0);
				}
				else
				{
					change_num(key, 408, 0);
				}
				break;
		  }
		  else if(!Touch_pressed&&(Touch_x >=339) && (Touch_x <=381) && (Touch_y >= 141) && (Touch_y <= 271))
		  {
				Touch_x=0;
				Touch_y=0;
				check=!check;
				key=7;
				if(check==1)
				{
					change_num(key, 326, 0);
				}
				else
				{
					change_num(key, 408, 0);
				}
				break;
		  }
		  else if(!Touch_pressed&&(Touch_x >=387) && (Touch_x <=429) && (Touch_y >= 141) && (Touch_y <= 271))
		  {
				Touch_x=0;
				Touch_y=0;
				check=!check;
				key=8;
				if(check==1)
				{
					change_num(key, 326, 0);
				}
				else
				{
					change_num(key, 408, 0);
				}
				break;
		  }
		  else if(!Touch_pressed&&(Touch_x >=435) && (Touch_x <=478) && (Touch_y >= 141) && (Touch_y <= 271))
		  {
				Touch_x=0;
				Touch_y=0;
				check=!check;
				key=9;
				if(check==1)
				{
					change_num(key, 326, 0);
				}
				else
				{
					change_num(key, 408, 0);
				}
				break;
		  }
      }
      
      Uart_Printf(" %d", key);
      if(check==1)
      {
            change_num(key, 326, 0);
      }
      else
      {
            change_num(key, 408, 0);
      }
      		
      
      
      if(key >= 0 && key < 10);
      {
         num[count] = key;
         count++;
      }
   }
   return num[0]*10 + num[1];
}

int change_num(int a, int b, int c)
{
	if(c == 1)
	{
		Lcd_Draw_BMP(0, 0, googookey);
	}
	switch(a)
   {
   	case 0:
   	Lcd_Draw_RMBG_BMP(b, 0, zero);
   	break;
   		
  	case 1:
  	Lcd_Draw_RMBG_BMP(b, 0, one);
  	break;
  	
  	case 2:
   	Lcd_Draw_RMBG_BMP(b, 0, two);
   	break;
   		
  	case 3:
  	Lcd_Draw_RMBG_BMP(b, 0, three);
  	break;
  	
  	case 4:
   	Lcd_Draw_RMBG_BMP(b, 0, four);
   	break;
   		
  	case 5:
  	Lcd_Draw_RMBG_BMP(b, 0, five);
  	break;
  	
  	case 6:
   	Lcd_Draw_RMBG_BMP(b, 0, six);
   	break;
   		
  	case 7:
  	Lcd_Draw_RMBG_BMP(b, 0, seven);
  	break;
  	
  	case 8:
   	Lcd_Draw_RMBG_BMP(b, 0, eight);
   	break;
   		
  	case 9:
  	Lcd_Draw_RMBG_BMP(b, 0, nine);
  	break;
   }
}
#include "buzzer.h"
#include "uart.h"
#include "abspitch.h"
#include <stdlib.h>

#include ".\LCD\music_clear.c"
#include ".\LCD\music_correct.c"
#include ".\LCD\music_wrong.c"
#include ".\LCD\music_main.c"
#include ".\LCD\music_key.c"

#ifndef printf 
#define printf Uart_Printf
#endif

#define TEST

static int answerArr[MAX_ANSWER_LENGTH]= 
{
   DOH , LAE , MI , PA, SOL, RA, SI, DOH, LAE, MI
};
static int soundArr[] = { DOH , LAE , MI , PA, SOL, RA, SI, _DOH, 
                    DOH_S,LAE_S,PA_S,SOL_S,RA_S,_DOH_S };
                    



#ifndef TEST
unsigned int seed=0;
#endif


extern volatile int Touch_pressed;
extern volatile int Touch_x, Touch_y;   


//main function of Absolute Pitch Game.
void start_musicGame()
{
   int res;
   
   printWelcomeMSG();
   printKeyMap();
   printf("\nSelect Mode\nSELECT1 : free play SELECT2 : game\n");
   
   Lcd_Draw_BMP(0, 0, music_main);
   
   while(1)
   {
      if(Touch_pressed)
      {
         if(Touch_x >= 20 && Touch_x <=154)
         {
            
            if(Touch_y>=20 && Touch_y <=101)
            {   
               backSlash(49);
               freeplay();
               break;
            }
            else if(Touch_y>=149 && Touch_y<=235)
            {
               backSlash(49);
               gameplay();
               break;
            }
         }
      }
      else
      {
         res = Key_Get_Pressed();
         if( res == 11 || res ==10 )
            break;
      }
   
      
      
      #ifndef TEST
      seed++;
      #endif
      
      if( res == 10 )
      {
         backSlash(49);
         gameplay();
         break;
      }
      else if( res == 11 )
      {
         backSlash(49);
         freeplay();
         break;
      }
   }
   
   printf("Bye!\n");
   return;
}


void freeplay()
{
   int ret=0;
   int ret_before=0;
   printf("freeplay! To quit, press SELECT1 or SELECT2 button");

   Lcd_Draw_BMP(0, 0, music_key);
   Key_Wait_Get_Released();
   
   while(1)
   {
      if(Touch_pressed)
         ret = checkKeyBoard_f(Touch_x,Touch_y);
      else if( (ret = Key_Get_Pressed()) )
      {
         if(ret==11 || ret==10)
         {
            backSlash(52);
            printf("Quit freeplay Mode...\n");   
            break;
         }
      }
      else
      {
         ret=0;
         if(ret_before != ret)
         {
            colorKeyNote(ret_before,0xfffe);
            ret_before=0;   
         }
      }
      
      
      if( ret!=0 )
      {
         if(ret_before != ret)
         {   
            colorKeyNote(ret_before,0xfffe);
            colorKeyNote(ret,0xafafaf);
            ret_before=ret;   
         }
         buzzer_play(soundArr[ret-2],80);
      }
      else
      {
         buzzer_off();   
         continue;
      }
   }
}
void gameplay()
{
   int i;
   int res;
   
   #ifndef TEST
   for(i=0;i<MAX_ANSWER_LENGTH;i++)
   {
      answerArr[i] = soundArr[(rand()+seed)%8];
   }
   #endif
   
   while(1)
   {
      //시작
      //LOOP, 중간에 틀리면 BREAK
      for(i=0; i<MAX_ANSWER_LENGTH; i++)
      {
         Lcd_Draw_BMP(0, 0, music_key);
         
         printf("Level %d..",i+1);
         playAnswer(i);
         
         
         res = checkAnswer(i);
         if(res==EXIT)
            return;
         
         if(res==0)
         {
            wrongsound();
            break;
         }
         else
         {
            //Correct ! congrets
            Lcd_Draw_BMP(0, 0, music_correct);
            correctSound();
            continue;
         }
      }
      //게임 종료
      if(i==MAX_ANSWER_LENGTH)
      {
         Lcd_Draw_BMP(0, 0, music_clear);
         printf("****GAME CLEAR!****\n");
         
         for(i = 0; i < 0x1ffffff; i++);
         break;
      }
      else
      {
         Lcd_Draw_BMP(0, 0, music_wrong);
         printf("****GAME OVER!****\n");
         
         for(i = 0; i < 0x1ffffff; i++);
         
         break;
      }
         
   }
}

void printWelcomeMSG()
{
      printf("  *********************************\n");
      printf("  *            WELCOME            *\n");
      printf("  *********************************\n");
}

void printKeyMap()
{
   printf(" Key Mapping Info ..\n");
   Uart_Printf("  O\t  O\t  O\t  O\t  O\n");
   Uart_Printf("sw3=D\tsw5=F\tsw7=A\tsw9=C_\tsw11= SELECT1\n");
   
   Uart_Printf("  O\t  O\t  O\t  O\t  O\n");
   Uart_Printf("sw2=C\tsw4=E\tsw6=G\tsw8=B\tsw10= SELECT2\n");
}

void playAnswer(int idx)
{
   int i;
   //CAUTION!!! index must be reached till same as param.
   for(i=0;i<=idx;i++)
   {
      buzzer_play(answerArr[i],200);
   }
}

int checkAnswer(int idx)
{
   int i;
   int res;
   int res_before;
   //CAUTION!!! index must be reached till same as param.
   
   while( Touch_pressed != 0 )
      {
      }
   
   for(i=0;i<=idx;i++)
   {
      Key_Wait_Get_Released();
      
      colorKeyNote(res_before,0xfffe);
      
      while(1)
      {
         if(Touch_pressed)
         {
            res = checkKeyBoard_g(Touch_x);   
            break;
         }
         else if( (res = Key_Get_Pressed()) )
         {
            if( res == 11 || res ==10 )
            {
               return EXIT;
            }
            else if( res !=0 )
               break;
         }
      }
      res_before=res;
      colorKeyNote(res,0xafafaf);
      
      if( soundArr[res-2] == answerArr[i])
      {
         buzzer_play(soundArr[res-2],200);
         continue;
      }
      else
      {
         backSlash(8 + (idx/10+1) );
         return ANS_WRONG;
      }
   }
   buzzer_delay(100);
   backSlash(8 + ((idx+1)/10+1) );
   return ANS_CORRECT;
   
}

void correctSound()
{
   buzzer_play(DOH,80);
   buzzer_play(MI,150);
   buzzer_play(SOL,200);
   buzzer_play(_DOH,350);
   buzzer_delay(1000);
}

void wrongsound()
{
   buzzer_play(WRONG_ANS_SOUND,50);
   buzzer_play(WRONG_ANS_SOUND,50);
}

void backSlash(int count)
{
   int i;
   for(i=0;i<count;i++)
      printf("\b");
}

int checkKeyBoard_g(int x)
{
	/* to reduce average compare Time..*/
   if(x>=238)
   {
      if(x<=302)
         return 6;
      else if(x<=364)
         return 7;
      else if(x<=420)
         return 8;
      else 
         return 9;
   }
   else
   {
      if(x>=175)
         return 5;
      else if(x>=119)
         return 4;
      else if(x>=58)
         return 3;
      else
         return 2;
   }
}

int checkKeyBoard_f(int x,int y)
{
   int ret;
   
   if(y>167)
   {
      ret = checkKeyBoard_g(x);
      return ret;
   }
   else
   {
      if(x>=267)
      {
         if(x<335)
            ret = 13;
         else if(x<421)
            ret = 14;
         else
            return 15;
      }
      else
      {
         if(x>175)
            ret = 12;
         else if(x>86)
            ret = 11;
         else
            ret = 10;
      }
   }
   return ret;
}


void colorKeyNote(int note,int color)
{
   if(note>9 && color == 0xfffe)
      color = 0x0000;

   if(note>8)
   {
      switch(note)
      {
         case 9:
            Lcd_Fill_Rectangular(423,3,453,166,color);
            Lcd_Fill_Rectangular(423,167,475,268,color);
            break;
         case 10:
            Lcd_Fill_Rectangular(33,3,70,166,color);
            break;
         case 11:
            Lcd_Fill_Rectangular(106,3,142,166,color);
            break;
         case 12:
            Lcd_Fill_Rectangular(214,3,250,166,color);
            break;
         case 13:
            Lcd_Fill_Rectangular(281,3,321,166,color);
            break;
         case 14:
            Lcd_Fill_Rectangular(350,3,386,166,color);
            break;
         case 15:
            Lcd_Fill_Rectangular(454,3,479,166,color);
            break;
      }
      
   }
   else
   {
      switch(note)
      {
         case 8:
            Lcd_Fill_Rectangular(387,3,418,166,color);
            Lcd_Fill_Rectangular(365,167,418,268,color);
            break;
         case 7:
            Lcd_Fill_Rectangular(322,3,349,167,color);
            Lcd_Fill_Rectangular(304,167,359,268,color);
            break;
         case 6:
            Lcd_Fill_Rectangular(251,3,281,167,color);
            Lcd_Fill_Rectangular(240,168,299,268,color);
            break;
         case 5:
            Lcd_Fill_Rectangular(178,3,213,167,color);
            Lcd_Fill_Rectangular(178,168,235,268,color);
            break;
         case 4:
            Lcd_Fill_Rectangular(143,3,173,167,color);
            Lcd_Fill_Rectangular(121,167,173,268,color);
            break;
         case 3:
            Lcd_Fill_Rectangular(71,3,106,167,color);
            Lcd_Fill_Rectangular(60,168,116,268,color);
            break;
         case 2:
            Lcd_Fill_Rectangular(2,3,32,167,color);
            Lcd_Fill_Rectangular(2,168,54,268,color);
            break;
         
      }
   }
}
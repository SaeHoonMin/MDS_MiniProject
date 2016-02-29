#include ".\LCD\main_text.c"
#include ".\LCD\main.c"
#include ".\LCD\menu.c"
#include ".\LCD\main_Game_Boy.c"
#include ".\LCD\main_press_to_continue.c"
#include ".\LCD\menu_selection.c"

extern volatile int Touch_pressed;
extern volatile int Touch_x, Touch_y;		// Calibration 정보 저장 값

#define BLACK	0x0000
#define WHITE	0xfffe
#define BLUE	0x003e
#define GREEN	0x07c0
#define RED		0xf800
#define YELLOW	0xffc0
#define main_time 0xfffff

int menu_pgm(void);
int start_pgm(int *mario, int *delay);

int start_pgm(int *mario, int *delay)
{
	int i=0, res;
	Lcd_Draw_BMP(0, 0, main);
	//for(i=0;i<main_time;i++);
	Lcd_Draw_RMBG_BMP(0, 0, main_Game_Boy);
	//for(i=0;i<main_time;i++);
	Lcd_Draw_RMBG_BMP(0, 0, main_press_to_continue);
	while(1)
	{	
		if(mario[i]!=0)
			buzzer_play(mario[i],delay[i]);
		else
			buzzer_delay(delay[i]);
		i++;
		/*
		if( (sizeof(mario)/sizeof(int)) == i)
		{
				i = 0;
		}
		*/
		if(Touch_pressed==1)
		{
			break;
		}
	}
	res=menu_pgm();
	return res;
}

int menu_pgm(void)
{
	Lcd_Clr_Screen(BLACK);
	Lcd_Draw_BMP(0, 0, menu);
	Lcd_Draw_RMBG_BMP(0, 0, menu_selection);
	
	Touch_x=0;
	Touch_y=0;

	while(1)
	{
		if(!Touch_pressed&&(Touch_x >=173) && (Touch_x <=311) && (Touch_y >= 79) && (Touch_y <= 121))
		{
			Touch_x=0;
			Touch_y=0;
			return 1;
		}
		else if(!Touch_pressed&&(Touch_x >=173) && (Touch_x <=311) && (Touch_y >= 130) && (Touch_y <= 172))
		{
			Touch_x=0;
			Touch_y=0;
			return 2;
		}
		else if(!Touch_pressed&&(Touch_x >=173) && (Touch_x <=311) && (Touch_y >= 181) && (Touch_y <= 224))
		{
			Touch_x=0;
			Touch_y=0;
			return 3;
		}
		
	}

}
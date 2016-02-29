#include "2440addr.h"
#include "option.h"
#include "macro.h"
#include "stdarg.h"
#include "HANTABLE.H"
#include "ENG8X16.H"
#include "HAN16X16.H"
//LCD config

#define MODE_TFT_16BIT    (0x4104)

#define LCD_XSIZE 		(480)	
#define LCD_YSIZE 		(272)

#define SCR_XSIZE 	(LCD_XSIZE)
#define SCR_YSIZE 	(LCD_YSIZE)

#define HOZVAL		(LCD_XSIZE-1)
#define LINEVAL		(LCD_YSIZE-1)

#define CLKVAL  	10	   

#define VBPD (10) //10 32-1
#define VFPD (10) //10 11-1
#define VSPW (5) //5 2-1
#define HBPD (10) //10 48-1 0으로 변경
#define HFPD (10) //10 16-1
#define HSPW (40) //40 96-1

// Macro

#define COPY(A,B) for(loop=0;loop<32;loop++) *(B+loop)=*(A+loop);
#define OR(A,B) for(loop=0;loop<32;loop++) *(B+loop)|=*(A+loop);

// LCD

void Lcd_Init(void);
void Lcd_Envid_On_Off(int onoff);
void Lcd_Power_Enable(int invpwren,int pwren);
void Lcd_Port_Init(void);
void Lcd_Set_Address(unsigned int fp);

// Graphic

void Lcd_Han_Putch(int x,int y,int color,int bkcolor,int data, int zx, int zy);
void Lcd_Eng_Putch(int x,int y,int color,int bkcolor,int data, int zx, int zy);
void Lcd_Puts(int x, int y, int color, int bkcolor, char *str, int zx, int zy);

void Graphic_Init(void);
void Lcd_Wait_Blank(void);
void Lcd_Copy(unsigned from, unsigned int to);
void Lcd_Select_Frame_Buffer(unsigned int id);
void Lcd_Display_Frame_Buffer(unsigned int id);
void Lcd_Set_Tran_Mode(int mode);
void Lcd_Put_Pixel(int x,int y,int c);
void Lcd_Clr_Screen(unsigned long color);
void Lcd_Hline(int y, int x1, int x2, int color);
void Lcd_Vline(int x, int y1, int y2, int color);
void Lcd_Line(int x1,int y1,int x2,int y2,int color);
void Lcd_Rectangular(int x1, int y1, int x2, int y2, int color);
void Lcd_Fill_Rectangular(int x1, int y1, int x2, int y2, int color);
void Lcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...);
void Lcd_Get_Info_BMP(int * x, int  * y, const unsigned char *fp);
void Lcd_Draw_BMP(int x, int y, const unsigned char *fp);
void Lcd_Draw_RMBG_BMP(int x, int y, const unsigned char *fp);

static unsigned char _first[]={0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 };
static unsigned char _middle[]={0,0,0,1,2,3,4,5,0,0,6,7,8,9,10,11,0,0,12,13,14,15,16,17,0,0,18,19,20,21};
static unsigned char _last[]={0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,0,17,18,19,20,21,22,23,24,25,26,27};
static unsigned char cho[]={0,0,0,0,0,0,0,0,0,1,3,3,3,1,2,4,4,4,2,1,3,0};
static unsigned char cho2[]={0,5,5,5,5,5,5,5,5,6,7,7,7,6,6,7,7,7,6,6,7,5};
static unsigned char jong[]={0,0,2,0,2,1,2,1,2,3,0,2,1,3,3,1,2,1,3,3,1,1};

// 4MB align 필요 (0x400000의 배수)
static unsigned int Fbuf[3] = {0x33400000, 0x33800000, 0x33c00000};
unsigned int (* Fb_ptr)[LCD_XSIZE/2]; 



static int Trans_mode;

//static unsigned short bfType;
//static unsigned int bfSize;
static unsigned int bfOffbits;
static unsigned int biWidth, biWidth2;
static unsigned int biHeight;

// Functions

void Lcd_Envid_On_Off(int onoff)
{
	(onoff) ? (rLCDCON1 |= 1) : (rLCDCON1 &= ~0x1);
}    

void Lcd_Power_Enable(int invpwren,int pwren)
{
	(void)invpwren;
	(void)pwren;
}    

void Lcd_Port_Init(void)
{
	rGPGDAT = rGPGDAT |(0x1<<4);
	rGPGCON = rGPGCON & ~(0x3<<8)|(0x1<<8);
	rGPCUP=0xffffffff; 
	rGPCCON=0xaaaaaaaa;
	rGPDUP=0xffffffff; 
	rGPDCON=0xaaaaaaaa;
}

void Graphic_Init(void)
{
     Lcd_Set_Address((unsigned int)0x33800000);
     Lcd_Port_Init();
     Lcd_Init();
     Lcd_Power_Enable(1,1);
     Lcd_Envid_On_Off(1);    
}

void Lcd_Init(void)
{
	rLCDCON1=(CLKVAL<<8)+(0<<7)+(0x3<<5)+(0xc<<1);
	// TFT LCD panel,12bpp TFT,ENVID=off
	rLCDCON2=(VBPD<<24)+(LINEVAL<<14)+(VFPD<<6)+(VSPW);
	rLCDCON3=(HBPD<<19)+(HOZVAL<<8)+(HFPD);
	rLCDCON4=(HSPW);
	rLCDINTMSK |= 0x3;
	rTCONSEL &= ~0x7;
	rTPAL=0;
     	Lcd_Select_Frame_Buffer(1);
     	Lcd_Display_Frame_Buffer(1);
}

#define M5D(n) ((n) & 0x1fffff)

void Lcd_Set_Address(unsigned int fp)
{
	rLCDSADDR1=((fp>>22)<<21);
	rLCDSADDR2=M5D(fp+(LCD_XSIZE*LCD_YSIZE));
	rLCDSADDR3=LCD_XSIZE;
//	rLCDSADDR1=(((unsigned int)fp>>22)<<21)+M5D((unsigned int)fp>>1);
//	rLCDSADDR2=M5D( ((unsigned int)fp+(SCR_XSIZE*LCD_YSIZE*2))>>1 );
//	rLCDSADDR3=(((SCR_XSIZE-LCD_XSIZE)/1)<<11)+(LCD_XSIZE/1);
}

void Lcd_Wait_Blank(void)
{
	while(rLCDCON5 & 1<<16);
}

void Lcd_Copy(unsigned from, unsigned int to)
{
	unsigned int i;

	for(i=0; i< (LCD_XSIZE / 2 * LCD_YSIZE) ; i++)
	{
		*((unsigned int *)(Fbuf[to])+i) = *((unsigned int *)(Fbuf[from])+i);
	}
}

void Lcd_Select_Frame_Buffer(unsigned int id)
{
     	Fb_ptr = (unsigned int (*)[LCD_XSIZE/2])Fbuf[id];
}

void Lcd_Display_Frame_Buffer(unsigned int id)
{
	Lcd_Set_Address(Fbuf[id]);
}

__inline void Lcd_Put_Pixel(int x,int y,int color)
{/*
    if(!Trans_mode)
    {
    	if((x%2)) // odd number
    	{
        	Fb_ptr[y][x/2]=( Fb_ptr[y][x/2] & 0xffff0000) | (color & 0x0000ffff);
        }
        else
        {
        	Fb_ptr[y][x/2]=( Fb_ptr[y][x/2] & 0x0000ffff) | ((color & 0x0000ffff) << 16);
        }
    }
    
    else
    {
    	if((x%2)) // odd number
    	{
        	Fb_ptr[y][x/2]= Fb_ptr[y][x/2] | (color & 0x0000ffff);
        }
        else
        {
        	Fb_ptr[y][x/2]= Fb_ptr[y][x/2] | ((color & 0x0000ffff) << 16);
        }
    }*/
    	switch((x%2)) // odd number
    	{
    		case 1:Fb_ptr[y][x/2]=( Fb_ptr[y][x/2] & 0xffff0000) | (color & 0x0000ffff);		break;
    		case 0:Fb_ptr[y][x/2]=( Fb_ptr[y][x/2] & 0x0000ffff) | ((color & 0x0000ffff) << 16);break;
        }
}
// Lcd_SetTranMode
void Lcd_Set_Tran_Mode(int mode)
{
	if(mode) Trans_mode = 1;
	else Trans_mode = 0;	
}

// Clear screen
void Lcd_Clr_Screen(unsigned long color)
{
	int i,j;

	for(i=0;i<SCR_YSIZE;i++)
	{
		for(j=0;j<SCR_XSIZE;j++)
		{
	        Lcd_Put_Pixel(j,i,(int)color);
		}
	}
}

// Lcd_Line
void Lcd_Line(int x1,int y1,int x2,int y2,int color)
{
	int dx,dy,e;
	dx=x2-x1; 
	dy=y2-y1;
    
	if(dx>=0)
	{
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 1/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 2/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 8/8 octant
			{
				e=dy-dx/2;
				while(x1<=x2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1+=1;
					e+=dy;
				}
			}
			else		// 7/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){x1+=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
	else //dx<0
	{
		dx=-dx;		//dx=abs(dx)
		if(dy >= 0) // dy>=0
		{
			if(dx>=dy) // 4/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){y1+=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 3/8 octant
			{
				e=dx-dy/2;
				while(y1<=y2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1+=1;
					e+=dx;
				}
			}
		}
		else		   // dy<0
		{
			dy=-dy;   // dy=abs(dy)

			if(dx>=dy) // 5/8 octant
			{
				e=dy-dx/2;
				while(x1>=x2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){y1-=1;e-=dx;}	
					x1-=1;
					e+=dy;
				}
			}
			else		// 6/8 octant
			{
				e=dx-dy/2;
				while(y1>=y2)
				{
					Lcd_Put_Pixel(x1,y1,color);
					if(e>0){x1-=1;e-=dy;}	
					y1-=1;
					e+=dx;
				}
			}
		}	
	}
}

// Lcd_Hline
void Lcd_Hline(int y, int x1, int x2, int color)
{
     int i, xx1, xx2;
     
     if(x1<x2)
     {
     	xx1=x1;
     	xx2=x2;
     }
     else 
     {
     	xx1=x2;
     	xx2=x1;
     } 
     for(i=xx1;i<=xx2;i++)
     {
         Lcd_Put_Pixel(i,y,color);
     }
} 

// Lcd_Vline
void Lcd_Vline(int x, int y1, int y2, int color)
{
     int i, yy1, yy2;
     
     if(y1<y2)
     {
     	yy1=y1;
     	yy2=y2;
     }
     else 
     {
     	yy1=y2;
     	yy2=y1;
     }	
     for(i=yy1;i<=yy2;i++)
     {
         Lcd_Put_Pixel(x,i,color);	
     }
}	

// Lcd_Rectangular
void Lcd_Rectangular(int x1, int y1, int x2, int y2, int color)
{
     int xx1, yy1, xx2, yy2;
     
     if(x1<x2)
     {
     	xx1=x1;
     	xx2=x2;
     }
     else 
     {
     	xx1=x2;
     	xx2=x1;
     } 
     
     if(y1<y2)
     {
     	yy1=y1;
     	yy2=y2;
     } 
     else 
     {
     	yy1=y2;
     	yy2=y1;
     } 
	 
     Lcd_Hline(yy1,xx1,xx2,color);
     Lcd_Hline(yy2,xx1,xx2,color);
     Lcd_Vline(xx1,yy1,yy2,color);
     Lcd_Vline(xx2,yy1,yy2,color);
} 

// Lcd_Bar
void Lcd_Fill_Rectangular(int x1, int y1, int x2, int y2, int color)
{
     int i, j;
     int xx1, yy1, xx2, yy2;
     
     if(x1<x2)
     {
     	xx1=x1;
     	xx2=x2;
     }
     else 
     {
     	xx1=x2;
     	xx2=x1;
     }
     
     if(y1<y2)
     {
     	yy1=y1;
     	yy2=y2;
     } 
     else 
     {
     	yy1=y2;
     	yy2=y1;
     }
     
     for(i=yy1;i<=yy2;i++)
     {
         for(j=xx1;j<=xx2;j++)
         {
             Lcd_Put_Pixel(j,i,color);
         }
     }
}


// Lcd_Han_Putch
void Lcd_Han_Putch(int x,int y,int color,int bkcolor, int data, int zx, int zy)
{
	unsigned int first,middle,last;	
	unsigned int offset,loop;
	unsigned char xs,ys;
	unsigned char temp[32];
	unsigned char bitmask[]={128,64,32,16,8,4,2,1};     

	first=(unsigned)((data>>8)&0x00ff);
	middle=(unsigned)(data&0x00ff);
	offset=(first-0xA1)*(0x5E)+(middle-0xA1);
	first=*(HanTable+offset*2);
	middle=*(HanTable+offset*2+1);
	data=(int)((first<<8)+middle);    

	first=_first[(data>>10)&31];
	middle=_middle[(data>>5)&31];
	last=_last[(data)&31];     

	if(last==0)
	{
		offset=(unsigned)(cho[middle]*640); 
		offset+=first*32;
		COPY(han16x16+offset,temp);

		if(first==1||first==24) offset=5120;  
		else offset=5120+704;
		offset+=middle*32;
		OR(han16x16+offset,temp);
	}
	else 
	{
		offset=(unsigned)(cho2[middle]*640); 
		offset+=first*32;
		COPY(han16x16+offset,temp);

		if(first==1||first==24) offset=5120+704*2; 
		else offset=5120+704*3;
		offset+=middle*32;
		OR(han16x16+offset,temp);

		offset=(unsigned)(5120+2816+jong[middle]*896);
		offset+=last*32;
		OR(han16x16+offset,temp);
	}

	for(ys=0;ys<16;ys++)
	{
		for(xs=0;xs<8;xs++)
		{
			if(temp[ys*2]&bitmask[xs])
			{
				if( (zx==1)&&(zy==1) ) Lcd_Put_Pixel(x+xs,y+ys,color);
				else if( (zx==2)&&(zy==1) )
				{
					Lcd_Put_Pixel(x+2*xs,y+ys,color);
					Lcd_Put_Pixel(x+2*xs+1,y+ys,color);
				}
				else if( (zx==1)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+xs,y+2*ys,color);
					Lcd_Put_Pixel(x+xs,y+2*ys+1,color);
				}
				else if( (zx==2)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+2*xs,y+2*ys+1,color);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys,color);
					Lcd_Put_Pixel(x+2*xs,y+2*ys,color);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys+1,color);
				}
			}
			else
			{
				if( (zx==1)&&(zy==1) ) Lcd_Put_Pixel(x+xs,y+ys,bkcolor);
				else if( (zx==2)&&(zy==1) )
				{
					Lcd_Put_Pixel(x+2*xs,y+ys,bkcolor);
					Lcd_Put_Pixel(x+2*xs+1,y+ys,bkcolor);
				}
				else if( (zx==1)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+xs,y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+xs,y+2*ys+1,bkcolor);
				}
				else if( (zx==2)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+2*xs,y+2*ys+1,bkcolor);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+2*xs,y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys+1,bkcolor);
				}	   	
			}
		}

		for(xs=0;xs<8;xs++)
		{
			if(temp[ys*2+1]&bitmask[xs])
			{
				if( (zx==1)&&(zy==1) )
				Lcd_Put_Pixel(x+xs+8,y+ys,color);
				else if( (zx==2)&&(zy==1) ){
				Lcd_Put_Pixel(x+2*(xs+8),y+ys,color);
				Lcd_Put_Pixel(x+2*(xs+8)+1,y+ys,color);
				}
				else if( (zx==1)&&(zy==2) ){
				Lcd_Put_Pixel(x+(xs+8),y+2*ys,color);
				Lcd_Put_Pixel(x+(xs+8),y+2*ys+1,color);
				}
				else if( (zx==2)&&(zy==2) ){
				Lcd_Put_Pixel(x+2*(xs+8),y+2*ys+1,color);
				Lcd_Put_Pixel(x+2*(xs+8)+1,y+2*ys,color);
				Lcd_Put_Pixel(x+2*(xs+8),y+2*ys,color);
				Lcd_Put_Pixel(x+2*(xs+8)+1,y+2*ys+1,color);
				}
			}

			else
			{	   	
				if( (zx==1)&&(zy==1) ) Lcd_Put_Pixel(x+xs+8,y+ys,bkcolor);
				else if( (zx==2)&&(zy==1) )
				{
					Lcd_Put_Pixel(x+2*(xs+8),y+ys,bkcolor);
					Lcd_Put_Pixel(x+2*(xs+8)+1,y+ys,bkcolor);
				}
				else if( (zx==1)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+(xs+8),y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+(xs+8),y+2*ys+1,bkcolor);
				}
				else if( (zx==2)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+2*(xs+8),y+2*ys+1,bkcolor);
					Lcd_Put_Pixel(x+2*(xs+8)+1,y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+2*(xs+8),y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+2*(xs+8)+1,y+2*ys+1,bkcolor);
				}	   	
			}
		}
	}
}

// Lcd_Eng_Putch
void Lcd_Eng_Putch(int x,int y,int color,int bkcolor,int data, int zx, int zy)
{
	unsigned offset,loop;
	unsigned char xs,ys;
	unsigned char temp[32];
	unsigned char bitmask[]={128,64,32,16,8,4,2,1};     

	offset=(unsigned)(data*16);
	COPY(eng8x16+offset,temp);

	for(ys=0;ys<16;ys++)
	{
		for(xs=0;xs<8;xs++)
		{
			if(temp[ys]&bitmask[xs])
			{
				if( (zx==1)&&(zy==1) ) Lcd_Put_Pixel(x+xs,y+ys,color);
				else if( (zx==2)&&(zy==1) )
				{
					Lcd_Put_Pixel(x+2*xs,y+ys,color);
					Lcd_Put_Pixel(x+2*xs+1,y+ys,color);
				}
				else if( (zx==1)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+xs,y+2*ys,color);
					Lcd_Put_Pixel(x+xs,y+2*ys+1,color);
				}
				else if( (zx==2)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+2*xs,y+2*ys+1,color);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys,color);
					Lcd_Put_Pixel(x+2*xs,y+2*ys,color);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys+1,color);
				}
			} 
			else
			{
				if( (zx==1)&&(zy==1) ) Lcd_Put_Pixel(x+xs,y+ys,bkcolor);
				else if( (zx==2)&&(zy==1) )
				{
					Lcd_Put_Pixel(x+2*xs,y+ys,bkcolor);
					Lcd_Put_Pixel(x+2*xs+1,y+ys,bkcolor);
				}
				else if( (zx==1)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+xs,y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+xs,y+2*ys+1,bkcolor);
				}
				else if( (zx==2)&&(zy==2) )
				{
					Lcd_Put_Pixel(x+2*xs,y+2*ys+1,bkcolor);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+2*xs,y+2*ys,bkcolor);
					Lcd_Put_Pixel(x+2*xs+1,y+2*ys+1,bkcolor);
				}	   	
			} 
		}
	}
}

// Lcd_Puts
void Lcd_Puts(int x, int y, int color, int bkcolor, char *str, int zx, int zy)
{
     unsigned data;
   
     while(*str)
     {
        data=*str++;
        if(data>=128) 
        { 
             data*=256;
             data|=*str++;
             Lcd_Han_Putch(x, y, color, bkcolor, (int)data, zx, zy);
             x+=zx*16;
        }
        else 
        {
             Lcd_Eng_Putch(x, y, color, bkcolor, (int)data, zx, zy);
             x+=zx*8;
        }
     } 
} 

// Lcd_Printf
void Lcd_Printf(int x, int y, int color, int bkcolor, int zx, int zy, char *fmt,...)
{
	va_list ap;
	char string[256];

	va_start(ap,fmt);
	vsprintf(string,fmt,ap);
	Lcd_Puts(x, y, color, bkcolor, string, zx, zy);
	va_end(ap);
}

// Lcd_GetInfoBMP
void Lcd_Get_Info_BMP(int * x, int  * y, const unsigned char *fp)
{
	*x =*(unsigned short *)(fp+18);    
	*y =*(unsigned short *)(fp+22); 
}

// Lcd_DrawBMP
void Lcd_Draw_BMP(int x, int y, const unsigned char *fp)
{	
     register int xx=0, yy=0;
     register int Height, Width;	
     unsigned int tmp;
     unsigned char tmpR, tmpG, tmpB;
	
     //bfType=*(unsigned short *)(fp+0);
     //bfSize=*(unsigned short *)(fp+2);
     //tmp=*(unsigned short *)(fp+4);
     //bfSize=(tmp<<16)+bfSize;
     bfOffbits=*(unsigned short *)(fp+10);
     biWidth=*(unsigned short *)(fp+18);
     biHeight=*(unsigned short *)(fp+22);
     //biWidth2=(bfSize-bfOffbits)/biHeight;	
     fp+=bfOffbits;
     Height=biHeight;
     Width=biWidth;
     for(yy=-1;yy<Height-1;yy++)
     {
         for(xx=0;xx<Width;xx++)
         {
             tmpB=*(fp+(Height-yy)*Width*3+xx*3+0);
             tmpG=*(fp+(Height-yy)*Width*3+xx*3+1);
             tmpR=*(fp+(Height-yy)*Width*3+xx*3+2);

             /*
             tmpR>>=3;
             tmpG>>=3;
             tmpB>>=3;
             */
             //if(xx<biWidth2) Lcd_Put_Pixel(x+xx,y+yy,(tmpR<<11)+(tmpG<<6)+(tmpB<<1)+1);
             Lcd_Put_Pixel(x+xx,y+yy,((tmpR>>3)<<11)+((tmpG>>3)<<6)+((tmpB>>3)<<1)+1);
         }
     }
}

void Lcd_Draw_RMBG_BMP(int x, int y, const unsigned char *fp)
{	
     register int xx=0, yy=0;
     register int Height, Width;	
     unsigned int tmp;
     unsigned char tmpR, tmpG, tmpB;
	
     //bfType=*(unsigned short *)(fp+0);
     //bfSize=*(unsigned short *)(fp+2);
     //tmp=*(unsigned short *)(fp+4);
     //bfSize=(tmp<<16)+bfSize;
     bfOffbits=*(unsigned short *)(fp+10);
     biWidth=*(unsigned short *)(fp+18);
     biHeight=*(unsigned short *)(fp+22);
     //biWidth2=(bfSize-bfOffbits)/biHeight;	
     fp+=bfOffbits;
     Height=biHeight;
     Width=biWidth;
     for(yy=-1;yy<Height-1;yy++)
     {
         for(xx=0;xx<Width;xx++)
         {
             tmpB=*(fp+(Height-yy)*Width*3+xx*3+0);
             tmpG=*(fp+(Height-yy)*Width*3+xx*3+1);
             tmpR=*(fp+(Height-yy)*Width*3+xx*3+2);
			 if((tmpB==0xFF)&&(tmpG==0xFF)&&(tmpR==0xFF))	continue;
             /*
             tmpR>>=3;
             tmpG>>=3;
             tmpB>>=3;
             */
             //if(xx<biWidth2) Lcd_Put_Pixel(x+xx,y+yy,(tmpR<<11)+(tmpG<<6)+(tmpB<<1)+1);
             Lcd_Put_Pixel(x+xx,y+yy,((tmpR>>3)<<11)+((tmpG>>3)<<6)+((tmpB>>3)<<1)+1);
         }
     }
}
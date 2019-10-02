#include "snake.h"
#include "tftlcd.h"
#include "picture.h"

u8 key;
void LCD_ShowPictureEx(u16 x, u16 y, u16 wide, u16 high)
{
	u16 temp = 0;
	long tmp=0,num=0;
	LCD_Set_Window(x, y, x+wide-1, y+high-1);
	num = wide * high*2 ;
	do
	{  
		temp = pic[tmp + 1];
		temp = temp << 8;
		temp = temp | pic[tmp];
		LCD_WriteData_Color(temp);//ÖðµãÏÔÊ¾
		tmp += 2;
	}
	while(tmp < num);	
}
void show(void)
{LCD_Fill(0,0,11,300,GREEN);
	LCD_Fill(12,0,228,11,GREEN);
	LCD_Fill(12,289,228,300,GREEN);
	LCD_Fill(229,0,239,300,GREEN);
	LCD_DrawLine_Color( 0,  319,  240,  319, GREEN);
  LCD_ShowString("click to start", 10, 320,24,BLACK,WHITE);
	LCD_DrawLine_Color( 0,  353,  240,  353, GREEN);
	LCD_ShowString("status: run", 10, 355,12,BLACK,WHITE);
}

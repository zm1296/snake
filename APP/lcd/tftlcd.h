#ifndef _tftlcd_H
#define _tftlcd_H	
typedef  unsigned short   u16;
typedef  unsigned char   u8;
typedef unsigned  short uint16_t;
typedef  unsigned  int   u32;
//定义LCD彩屏的驱动类型  可根据自己手上的彩屏背面型号来选择打开哪种驱动
//#define TFTLCD_HX8357D 

//#define TFTLCD_HX8352C

//#define TFTLCD_ILI9341

//#define TFTLCD_ILI9327

//#define TFTLCD_ILI9486

//#define TFTLCD_R61509V
#define TFTLCD_R61509VN//

//#define TFTLCD_R61509V3

//#define TFTLCD_ST7793

//#define TFTLCD_NT5510

//#define TFTLCD_HX8357DN

//#define TFTLCD_ILI9325



#define TFTLCD_DIR	0	//0：竖屏  1：横屏  默认竖屏

//TFTLCD地址结构体
typedef struct
{
	u16 LCD_CMD;
	u16 LCD_DATA;
}TFTLCD_TypeDef;


//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
//注意设置16位总线时STM32内部会右移一位对齐!			    
#define TFTLCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define TFTLCD             ((TFTLCD_TypeDef *) TFTLCD_BASE)
  
//TFTLCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;            //LCD 方向
}_tftlcd_data;


//LCD参数
extern _tftlcd_data tftlcd_data;	//管理LCD重要参数
//LCD的前端颜色和背景色	   
extern u16  FRONT_COLOR;//前端颜色 默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色


//画笔颜色
#define WHITE     	 0xFFFF
#define BLACK   		 0x0000	  
#define BLUE     		 0x001F
#define BRED  			 0XF81F
#define GRED 				 0XFFE0
#define GBLUE				 0X07FF
#define RED          0xF800
#define MAGENTA      0xF81F
#define GREEN      	 0x07E0
#define CYAN       	 0x7FFF
#define YELLOW     	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色



void LCD_WriteCmd(u16 cmd);
void LCD_WriteData(u16 data);
void LCD_WriteCmdData(u16 cmd,u16 data);
void LCD_WriteData_Color(u16 color);

void TFTLCD_Init(void);           //初始化
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);
void LCD_Clear(u16 Color);	 												//清屏
void LCD_Fill(u16 xState,u16 yState,u16 xEnd,u16 yEnd,u16 color);
void LCD_DrawPoint(u16 x,u16 y);//画点
void LCD_DrawFRONT_COLOR(u16 x,u16 y,u16 color);
u16 LCD_ReadPoint(u16 x,u16 y);
void LCD_ShowFontHZ1(u16 x, u16 y, u8 *cn);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawLine_Color(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void LCD_DrowSign(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle_color(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high,u8 *pic);
/*
  函数原型:void LCD_ShowChar(char ch,int x,int y,u16 fg_color,u16 bg_color)
	功    能：显示单字符
	参    数： char ch
*/
void LCD_ShowChar(char ch,int x,int y,int size,u16 fg_color,u16 bg_color);
void LCD_ShowChina(char *str,int x,int y,u16 fg_color,u16 bg_color);
/*
   原型:void LCD_ShowString(char *str,int x,int y,int size,u16 fg_color,u16  bg_color);
   功能:显示字符 串
   参数:
        char *ch 显示字符串
		int x,y,显示位置
		int size 字大小	(哪种点整 12/16/24)
		u16 fg_color,bg_color 前景色 ，背景色
*/
void LCD_ShowString(char *str,int x,int y,int size,u16 fg_color,u16  bg_color);
/*
    原型:void LCD_ShowChina(char *str,int x,int y,u16 fg_color,u16  bg_color);
   功能:显示中文字符串
   参数:
        char *str 显示符中文字符串
		int x,y,显示位置
		u16 fg_color,bg_color 前景色 ，背景色
*/
void LCD_ShowChinaStr(char *str,int x,int y,u16 fg_color,u16  bg_color);

void LCD_ShowFontHZ16(u16 x, u16 y, u8 *cn,u16 color1);
void LCD_ShowFontHZ(u16 x, u16 y, u8 *cn);
#endif  

	 




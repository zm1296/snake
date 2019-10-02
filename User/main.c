#include "system.h"
#include "SysTick.h"
#include "stdlib.h"
#include "led.h"
#include "beep.h"
#include "tftlcd.h"
#include "mybmp.h"
#include "play.h"
#include "key.h"
#include "touch.h"
#include "time.h"
#include "led.h"
#include "rtc.h" 
#include "wkup.h"
#include "gameover.h"
#include "snake.h"
#define Delay500ms() delay_ms(500)
#define SNAKE_Max_Long 50//蛇的最大长度 
u8 pause=0;
u8 start=0;
char str1[32]={0};
void display_init() 
	{
	FRONT_COLOR=RED;
	//LCD_ShowString(tftlcd_data.width-8*4,0,tftlcd_data.width,tftlcd_data.height,16,"RST");
	LCD_Fill(120, tftlcd_data.height - 16, 139, tftlcd_data.height, BLUE);
    LCD_Fill(140, tftlcd_data.height - 16, 159, tftlcd_data.height, RED);
    LCD_Fill(160, tftlcd_data.height - 16, 179, tftlcd_data.height, MAGENTA);
    LCD_Fill(180, tftlcd_data.height - 16, 199, tftlcd_data.height, GREEN);
    LCD_Fill(200, tftlcd_data.height - 16, 219, tftlcd_data.height, CYAN);
    LCD_Fill(220, tftlcd_data.height - 16, 239, tftlcd_data.height, YELLOW);
}


//蛇
struct Snake
{
	s16 X[SNAKE_Max_Long];
	s16 Y[SNAKE_Max_Long];
	u8 Long;//蛇的长度
	u8 Life;//蛇的生命 0活着 1死亡
	u8 Direction;//蛇移动的方向
}snake;

//食物
struct Food
{
	u8 X;//食物横坐标
	u8 Y;//食物纵坐标
	u8 Yes;//判断是否要出现食物的变量 0有食物 1需要出现食物
}food;
void touch(void)
{	
	key=KEY_Scan(0);	
	
	if(key==KEY_UP&&snake.Direction!=4)		//上
	{
		snake.Direction=3;
	} 
	if(key==KEY_DOWN&&snake.Direction!=3)	   //下
	{
		snake.Direction=4;
	} 
	if(key==KEY_LEFT&&snake.Direction!=1)	   //左
	{
		snake.Direction=2;
	} 
	if(key==KEY_RIGHT&&snake.Direction!=2)	  //右
	{
		snake.Direction=1;
	}
}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET)
	{
		touch();
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	
	}
}  
void gameov()
{
	start=0;//停止游戏
	
  LCD_ShowString(" GAME OVER",13,13,24,BLACK,WHITE);//显示生命值	
	Delay500ms();
}
void playgame()
{
	u16 i,n;//i蛇的关节数 n用来判断食物和蛇的身体是否重合
	snake.Long=2;//定义蛇的长度
	snake.Life=0;//蛇还活着
	snake.Direction=1;//蛇的起始方向定义为右
	food.Yes=1;//出现新食物
	snake.X[0]=25;snake.Y[0]=25;
	snake.X[1]=25;snake.Y[1]=25;

	while(1)
	{
			if(food.Yes==1)//出现新的食物
			{
				while(1)
				{
						srand(calendar.sec);
						food.X=13+(int)(rand()%(19))*12;
						food.Y=13+(int)(rand()%(22))*12;
					  if (food.X<=13)food.X=13;
					  if (food.Y<=13)food.Y=13;
					if (food.X>=217)food.X=217;
					  if (food.Y>=217)food.Y=217;
						for(n=0;n<snake.Long;n++)
						{
							if(food.X==snake.X[n]&&food.Y==snake.Y[n])
								break;
						}
						if(n==snake.Long)
						food.Yes=0;	
						break;
					}
			}
				
				if(food.Yes==0)//有食物就要显示
				{	
					LCD_Fill(food.X,food.Y,food.X+9,food.Y+9,RED);
				}
				//取得需要重新画的蛇的节数
				for(i=snake.Long-1;i>0;i--)
				{
					snake.X[i]=snake.X[i-1];
					snake.Y[i]=snake.Y[i-1];
				}
				//通过按键来设置蛇的运动方向
				switch(snake.Direction)
				{
					case 1:snake.X[0]+=12;break;//向右运动
					case 2:snake.X[0]-=12;break;//向左运动
					case 3:snake.Y[0]-=12;break;//向上运动
					case 4:snake.Y[0]+=12;break;//向下运动
				}
				for(i=0;i<snake.Long;i++)//画出蛇	
				LCD_Fill(snake.X[i],snake.Y[i],snake.X[i]+9,snake.Y[i]+9,BLUE);//画蛇身体
				while(pause==1){};
				delay_ms(500);//延时
				LCD_Fill(snake.X[snake.Long-1],snake.Y[snake.Long-1],snake.X[snake.Long-1]+9,snake.Y[snake.Long-1]+9,WHITE);//消除蛇身		
						
					
				//判断是否撞墙
				if(snake.X[0]<13||snake.X[0]>222||snake.Y[0]<13||snake.Y[0]>282)
					snake.Life=1;//蛇死掉了
		
				//当蛇的身体超过3节后判断蛇自身的碰撞
				for(i=3;i<snake.Long;i++)
				{
					if(snake.X[i]==snake.X[0]&&snake.Y[i]==snake.Y[0])//自身的任一坐标值与蛇头坐标相等就认为是自身碰撞
						snake.Life=1;
				}
				if(snake.Life==1)//以上两种判断以后如果蛇死掉了跳出内循环，重新开始
				{
					gameov();
					break;
				}	
				//判断蛇是否吃到了食物
				if(snake.X[0]==food.X&&snake.Y[0]==food.Y)
				{
					LCD_Fill(food.X,food.Y,food.X+9,food.Y+9,GRAY);//把吃到的食物消除
					if(!((snake.Long==SNAKE_Max_Long)&&(snake.Long==SNAKE_Max_Long)))
					snake.Long++;//蛇的身体长一节
					food.Yes=1;//需要重新显示食物
				}	
		}	
	start=0;
}
void ui_1(void)
{

	LCD_Clear(WHITE);
  	LCD_ShowFontHZ16(20,20,"组名",BLACK);
	  LCD_ShowString(":Ikun", 55, 20,12,BLACK,WHITE);
		LCD_ShowFontHZ16(20,45,"组员",BLACK);
	  LCD_ShowString(":", 55, 45,12,BLACK,WHITE);
	  LCD_ShowFontHZ16(65,45,"张模",BLACK);
    LCD_ShowFontHZ16(100,45,"吴堃",BLACK);
   	LCD_ShowFontHZ16(138,45,"顾昌达",BLACK);
  	LCD_ShowFontHZ16(190,45,"王琦",BLACK);
	  LCD_ShowFontHZ16(20,70,"项目名称",BLACK);
  	LCD_ShowString(":", 86, 70,12,BLACK,WHITE);
		LCD_ShowFontHZ16(95,70,"贪吃蛇",BLACK);
  	//中部
    LCD_DrawLine_Color( 0,  100,  240,  100, GREEN);
		//中左
	  LCD_DrawRectangle_color(20, 140, 100, 220,CYAN);
	  LCD_ShowPicture(20,140,80,80,mybmp);
	  LCD_DrawRectangle_color(20, 230, 100, 250,CYAN);
		LCD_ShowString(" start ", 25, 235,12,BLACK,WHITE);
		//中右
	  LCD_DrawRectangle_color(120, 140, 200, 220,CYAN);
	  LCD_ShowPicture(120,140,80,80,mybmp);
	  LCD_DrawRectangle_color(120, 230, 200, 250,CYAN);
	  LCD_ShowString(" set ", 125, 235,12,BLACK,WHITE);
	  //底部
		LCD_DrawLine_Color( 0,  319,  240,  319, GREEN);
	  LCD_DrawLine_Color( 0,  300,  240,  300, GREEN);
	  LCD_ShowString("status: free ", 10, 320,12,BLACK,WHITE);			
}
void ui_2(void)
{
	      //二级界面
				LCD_ShowFontHZ16(20-10,45,"组名",BLACK);
      	LCD_ShowString(":Ikun", 55-10, 45,12,BLACK,WHITE);
				LCD_ShowFontHZ16(20-10,70,"分数",BLACK);
	      LCD_ShowString(":", 55-10, 70,12,BLACK,WHITE);
	      sprintf(str1,"%02d",snake.Long );
				LCD_ShowString(str1, 55, 73,12,BLACK,WHITE);
				LCD_ShowFontHZ16(20-10,20,"项目名称",BLACK);
	      LCD_ShowString(":", 86-10, 20,12,BLACK,WHITE);
		    LCD_ShowFontHZ16(95-10,20,"贪吃蛇",BLACK);
				LCD_DrawRectangle_color(170, 70, 236, 90,BLACK);
				LCD_ShowString(" end ", 172, 75,12,BLACK,WHITE);
				//第一条线
				LCD_DrawLine_Color( 0,  100,  240,  100, BLACK);
        LCD_ShowPicture(70,175,100,50,play);	
				//第二条线
        LCD_DrawLine_Color( 0,  300,  240,  300, BLACK);
  			LCD_DrawLine_Color( 0,  319,  240,  319, BLACK);
			 	LCD_ShowString("status: ready", 10, 355,12,BLACK,WHITE);						
}
void ui_3(void)
{
	//三级界面
  LCD_ShowPicture(70,30,100,100,gameover);
	LCD_DrawLine_Color( 0,  160,  240,  160, BLACK);
	LCD_ShowFontHZ16(10,220,"分数",BLACK);
	LCD_ShowString(":", 55, 220,12,BLACK,WHITE);
	LCD_DrawLine_Color( 0,  319,  240,  319, BLACK);
	LCD_DrawLine_Color( 0,  303,  240,  303, BLACK);
	sprintf(str1,"%02d",snake.Long );
	LCD_ShowString(str1, 65, 223,12,BLACK,WHITE);
	LCD_DrawLine_Color( 0,  353,  240,  353, BLACK);
  LCD_ShowString("status: end", 10, 320,24,BLACK,WHITE);
	LCD_DrawRectangle_color(19, 380, 105, 400,BLACK);
	LCD_ShowString(" return ", 20, 385,12,BLACK,WHITE);
}
int main()
{
	int a=0;
	u8 key;
	u16 i=0;
	u16 penColor = BLUE;
	SysTick_Init(72);
	LED_Init();
	KEY_Init();
	TFTLCD_Init();
	TOUCH_Init();
  LCD_Clear(WHITE);
	ui_1();
  RTC_Init();
	TIM4_Init(1000,36000-1);
	while(1)
	{
		switch(a)
			{case 0:
		   if(TOUCH_Scan() == 0)
				 { 
  					if(((TouchData.lcdx>20)&&(TouchData.lcdx<100))&& ((TouchData.lcdy>230)&&(TouchData.lcdy<250)))//start
  							{
								LCD_DrawRectangle_color(20, 230, 100, 250,YELLOW);
						    Delay500ms();
							  LCD_Clear(WHITE);
				        ui_2();
						    a=1;
						    while(1)
									{
		                if(TOUCH_Scan() == 0)
											 {if(((TouchData.lcdx>170)&&(TouchData.lcdx<236))&& ((TouchData.lcdy>70)&&(TouchData.lcdy<90)))//end
												 {
						              LCD_DrawRectangle_color(170, 70, 236, 90,RED);
						              Delay500ms();
							            LCD_Clear(WHITE);
                          ui_3();
													a=2;												
						              }
				               }break;
									}
							}
				 }break;
			case 1:
		  if(TOUCH_Scan() == 0)
				{
					if(((TouchData.lcdx>170)&&(TouchData.lcdx<236))&& ((TouchData.lcdy>70)&&(TouchData.lcdy<90)))//end
												 {
						              LCD_DrawRectangle_color(170, 70, 236, 90,YELLOW);
						              Delay500ms();
							            LCD_Clear(WHITE);
                          ui_3();
													a=2;
												 }
											 if(((TouchData.lcdx>70)&&(TouchData.lcdx<170))&& ((TouchData.lcdy>175)&&(TouchData.lcdy<225)))//开始范围
												 {
						            LCD_DrawRectangle_color(70, 175, 175, 225,YELLOW);
						              Delay500ms();
							            LCD_Clear(WHITE);
                          show();
													a=3;												
						              }
				 }break;
			case 2:
		         if(TOUCH_Scan() == 0)
							 {
								if(((TouchData.lcdx>20)&&(TouchData.lcdx<105))&& ((TouchData.lcdy>385)&&(TouchData.lcdy<400)))//return  
									{
						       LCD_DrawRectangle_color(19, 380, 105, 400,YELLOW);
						       Delay500ms();
							     LCD_Clear(WHITE);
                   ui_1();
									 a=0;
					        }
							 }break;
			case 3:
		         if(TOUCH_Scan() == 0)
							 {TIM3_Init(50,7199);
								RTC_Init();
							  LCD_Clear(WHITE);
								show();
								playgame();
						    while(1)
									{
 											 if(start==0){
						               LCD_Clear(WHITE);
                             ui_3();
									          a=2;											
						              }break;
				               
									}
							
				 }break;	
			 }
		 }
	 }		
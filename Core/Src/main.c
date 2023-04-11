/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "characters.h"
#include "turnMethods.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
#include "delay.h"
//#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
//#include "usmart.h"
#include "touch.h"
#include "24cxx.h"
#include "24l01.h" //通信驱动 基于spi进行通信
//#include "remote.h" 红外遥控驱动
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern unsigned char uRx_Data[1024];
extern unsigned char uLength;
/* 变量*/
int round = 0;
int getInstruction = -1;
uint8_t rxBuffer[20];
unsigned char info[1024];
// 玩家自己当下的属性显�??
extern struct character *thisTurnPlayer;
extern struct character *thisTurnEnemy;


extern unsigned char bmp_pika;// 皮卡丘头�??
extern unsigned char bmp_monster; // monster头像
extern unsigned char bmp_dragon;
extern unsigned char bmp_sword;
extern unsigned char bmp_bag;
extern unsigned char bmp_ability;
extern unsigned char bmp_run;

struct character A;
struct character B;
struct character Enemy;

struct skill A_skill[4];
struct skill B_skill[4];
struct item item[4];
// 总共有三个界面
int menu=1;

// mode=1我的回合
u8 key,mode;
u16 t=0;
u8 tmp_buf[33];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
unsigned char DATA_TO_SEND[800];
int state_num = 0;
u8 STATE[30];
UART_HandleTypeDef huart1;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//清空屏幕并在右上角显�???"RST"
void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//清屏
 	POINT_COLOR=BLUE;//设置字体为蓝�???
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
  	POINT_COLOR=RED;//设置画笔蓝色
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部�???
//画水平线
//x0,y0:坐标
//len:线长�???
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++)
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside
		{
 			if (x>imax)
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}
//两个数之差的绝对�???
//x1,x2：需取差值的两个�???
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{
	if(x1>x2)return x1-x2;
	else return x2-x1;
}
//画一条粗�???
//(x1,y1),(x2,y2):线条的起始坐�???
//size：线条的粗细程度
//color：线条的颜色

void screen_print(){
	LCD_Clear(WHITE);//清屏
	POINT_COLOR=BLUE;//设置字体为蓝�??
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
	// HP
	if(thisTurnPlayer==&A){
	sprintf(DATA_TO_SEND, "HP: %d",A.HP);
	LCD_ShowString(0,100,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "HP: %d",B.HP);
		LCD_ShowString(0,100,200,16,16, DATA_TO_SEND);
	}

	sprintf(DATA_TO_SEND, "HP: %d",Enemy.HP);
	LCD_ShowString(121,100,200,16,16, DATA_TO_SEND);
	//防御
	if(thisTurnPlayer==&A){
	sprintf(DATA_TO_SEND, "Defense: %d",A.DEF);
	LCD_ShowString(0,120,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "Defense: %d",B.DEF);
		LCD_ShowString(0,120,200,16,16, DATA_TO_SEND);
	}
	sprintf(DATA_TO_SEND, "Defense: %d",Enemy.DEF);
	LCD_ShowString(121,120,200,16,16, DATA_TO_SEND);
	//攻击
	if(thisTurnPlayer==&A){
	sprintf(DATA_TO_SEND, "Attack: %d",A.ATK);
	LCD_ShowString(0,140,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "Attack: %d",B.ATK);
		LCD_ShowString(0,140,200,16,16, DATA_TO_SEND);
	}
	sprintf(DATA_TO_SEND, "Attack: %d",Enemy.ATK);
	LCD_ShowString(121,140,200,16,16, DATA_TO_SEND);
	//名字
	if(thisTurnPlayer==&A){
	sprintf(DATA_TO_SEND, "Name: %s",A.name);
	LCD_ShowString(0,160,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "Name: %s",B.name);
		LCD_ShowString(0,160,200,16,16, DATA_TO_SEND);
	}
	sprintf(DATA_TO_SEND, "Name: %s",Enemy.name);
	LCD_ShowString(121,160,200,16,16, DATA_TO_SEND);
	//法力
	if(thisTurnPlayer==&A){
	sprintf(DATA_TO_SEND, "MP: %d",A.MP);
	LCD_ShowString(0,180,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "MP: %d",B.MP);
		LCD_ShowString(0,180,200,16,16, DATA_TO_SEND);
	}
	sprintf(DATA_TO_SEND, "MP: %d",Enemy.MP);
	LCD_ShowString(121,180,200,16,16, DATA_TO_SEND);


	LCD_ShowString(0, lcddev.height-24, 200, 16, 16, STATE);

	// 绘制游戏界面
	LCD_DrawLine(0,270,240,270);// �??个横�??
	LCD_DrawLine(120,0,120,320);//三个竖线
	LCD_DrawLine(60,270,60,320);
	LCD_DrawLine(180,270,180,320);

	// 绘制角色
	// 绘制自己角色
	if (thisTurnPlayer==&A){
		LCD_ShowPika(0,0,(u8 **)bmp_pika,80,100,1);
	}else if(thisTurnPlayer==&B){
		LCD_ShowMonster(0,0,(u8 **)bmp_monster,80,100,1);
	}
	// 绘制对手角色
	if(thisTurnEnemy==&A){
		LCD_ShowPika(120,0,(u8 **)bmp_pika,80,100,1);
	}else if(thisTurnEnemy==&B){
		LCD_ShowMonster(120,0,(u8 **)bmp_monster,80,100,1);
	}

	LCD_ShowSword(10,270,(u8 **)bmp_sword,40,25,1);
	LCD_ShowAbility(75,270,(u8 **)bmp_ability,40,25,1);
	LCD_ShowBag(190,270,(u8 **)bmp_bag,40,25,1);
	LCD_ShowRun(120,270,(u8 **)bmp_run,40,25,1);


	POINT_COLOR=RED;//设置画笔为红�??

}


void screen_norm_print(){
//	LCD_Clear(WHITE);//清屏
	POINT_COLOR=BLUE;//设置字体为蓝�??
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域

	// HP
	if(thisTurnPlayer==&A){
	sprintf(DATA_TO_SEND, "HP: %d",A.HP);
	LCD_ShowString(0,100,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "HP: %d",B.HP);
		LCD_ShowString(0,100,200,16,16, DATA_TO_SEND);
	}

	sprintf(DATA_TO_SEND, "HP: %d",Enemy.HP);
	LCD_ShowString(121,100,200,16,16, DATA_TO_SEND);
	//防御
	if(thisTurnPlayer==&A){
	sprintf(DATA_TO_SEND, "Defense: %d",A.DEF);
	LCD_ShowString(0,120,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "Defense: %d",B.DEF);
		LCD_ShowString(0,120,200,16,16, DATA_TO_SEND);
	}
	sprintf(DATA_TO_SEND, "Defense: %d",Enemy.DEF);
	LCD_ShowString(121,120,200,16,16, DATA_TO_SEND);
	//攻击
	if(thisTurnPlayer==&A){
	sprintf(DATA_TO_SEND, "Attack: %d",A.ATK);
	LCD_ShowString(0,140,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "Attack: %d",B.ATK);
		LCD_ShowString(0,140,200,16,16, DATA_TO_SEND);
	}
	sprintf(DATA_TO_SEND, "Attack: %d",Enemy.ATK);
	LCD_ShowString(121,140,200,16,16, DATA_TO_SEND);
	//名字
	if(thisTurnPlayer==&A){
	sprintf(DATA_TO_SEND, "Name: %s",A.name);
	LCD_ShowString(0,160,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "Name: %s",B.name);
		LCD_ShowString(0,160,200,16,16, DATA_TO_SEND);
	}
	sprintf(DATA_TO_SEND, "Name: %s",Enemy.name);
	LCD_ShowString(121,160,200,16,16, DATA_TO_SEND);
	//法力
	if(thisTurnPlayer==&A){
	sprintf(DATA_TO_SEND, "MP: %d",A.MP);
	LCD_ShowString(0,180,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "MP: %d",B.MP);
		LCD_ShowString(0,180,200,16,16, DATA_TO_SEND);
	}
	sprintf(DATA_TO_SEND, "MP: %d",Enemy.MP);
	LCD_ShowString(121,180,200,16,16, DATA_TO_SEND);

	// 回合
	if(mode==1){
		sprintf(DATA_TO_SEND, "Your Turn!");
		LCD_ShowString(0,200,200,16,16, DATA_TO_SEND);
	}else{
		sprintf(DATA_TO_SEND, "Your Turn!");
		LCD_ShowString(121,200,200,16,16, DATA_TO_SEND);
	}


	LCD_ShowString(0, lcddev.height-24, 200, 16, 16, STATE);

	// 绘制游戏界面
	LCD_DrawLine(0,270,240,270);// �??个横�??
	LCD_DrawLine(120,0,120,320);//三个竖线
	LCD_DrawLine(60,270,60,320);
	LCD_DrawLine(180,270,180,320);


	// 绘制自己角色
	if (thisTurnPlayer==&A){
		LCD_ShowPika(0,0,(u8 **)bmp_pika,80,100,1);
	}else if(thisTurnPlayer==&B){
		LCD_ShowMonster(0,0,(u8 **)bmp_monster,80,100,1);
	}
	// 绘制对手角色
	if(thisTurnEnemy==&A){
		LCD_ShowPika(120,0,(u8 **)bmp_pika,80,100,1);
	}else if(thisTurnEnemy==&B){
		LCD_ShowMonster(120,0,(u8 **)bmp_monster,80,100,1);
	}


	// 绘制图标
	LCD_ShowSword(10,270,(u8 **)bmp_sword,40,25,1);// nengli
	LCD_ShowAbility(75,270,(u8 **)bmp_ability,40,25,1);// huan ren
	LCD_ShowBag(190,270,(u8 **)bmp_bag,40,25,1);// bag
	LCD_ShowRun(120,270,(u8 **)bmp_run,40,25,1);// run

	POINT_COLOR=RED;//设置画笔为红�??

}

void screen_menu2_print(){//第二个界�??

		POINT_COLOR=BLUE;//设置字体为蓝�??
		LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域

		// HP
		if(thisTurnPlayer==&A){
		sprintf(DATA_TO_SEND, "HP: %d",A.HP);
		LCD_ShowString(0,100,200,16,16, DATA_TO_SEND);
		}else{
			sprintf(DATA_TO_SEND, "HP: %d",B.HP);
			LCD_ShowString(0,100,200,16,16, DATA_TO_SEND);
		}

		sprintf(DATA_TO_SEND, "HP: %d",Enemy.HP);
		LCD_ShowString(121,100,200,16,16, DATA_TO_SEND);
		//防御
		if(thisTurnPlayer==&A){
		sprintf(DATA_TO_SEND, "Defense: %d",A.DEF);
		LCD_ShowString(0,120,200,16,16, DATA_TO_SEND);
		}else{
			sprintf(DATA_TO_SEND, "Defense: %d",B.DEF);
			LCD_ShowString(0,120,200,16,16, DATA_TO_SEND);
		}
		sprintf(DATA_TO_SEND, "Defense: %d",Enemy.DEF);
		LCD_ShowString(121,120,200,16,16, DATA_TO_SEND);
		//攻击
		if(thisTurnPlayer==&A){
		sprintf(DATA_TO_SEND, "Attack: %d",A.ATK);
		LCD_ShowString(0,140,200,16,16, DATA_TO_SEND);
		}else{
			sprintf(DATA_TO_SEND, "Attack: %d",B.ATK);
			LCD_ShowString(0,140,200,16,16, DATA_TO_SEND);
		}
		sprintf(DATA_TO_SEND, "Attack: %d",Enemy.ATK);
		LCD_ShowString(121,140,200,16,16, DATA_TO_SEND);
		//名字
		if(thisTurnPlayer==&A){
		sprintf(DATA_TO_SEND, "Name: %s",A.name);
		LCD_ShowString(0,160,200,16,16, DATA_TO_SEND);
		}else{
			sprintf(DATA_TO_SEND, "Name: %s",B.name);
			LCD_ShowString(0,160,200,16,16, DATA_TO_SEND);
		}
		sprintf(DATA_TO_SEND, "Name: %s",Enemy.name);
		LCD_ShowString(121,160,200,16,16, DATA_TO_SEND);
		//法力
		if(thisTurnPlayer==&A){
		sprintf(DATA_TO_SEND, "MP: %d",A.MP);
		LCD_ShowString(0,180,200,16,16, DATA_TO_SEND);
		}else{
			sprintf(DATA_TO_SEND, "MP: %d",B.MP);
			LCD_ShowString(0,180,200,16,16, DATA_TO_SEND);
		}
		sprintf(DATA_TO_SEND, "MP: %d",Enemy.MP);
		LCD_ShowString(121,180,200,16,16, DATA_TO_SEND);


		LCD_ShowString(0, lcddev.height-24, 200, 16, 16, STATE);

		// 绘制游戏界面
		LCD_DrawLine(0,270,240,270);// �??个横�??
		LCD_DrawLine(120,0,120,320);//三个竖线
		LCD_DrawLine(60,270,60,320);
		LCD_DrawLine(180,270,180,320);


		// 绘制自己角色
		if (thisTurnPlayer==&A){
			LCD_ShowPika(0,0,(u8 **)bmp_pika,80,100,1);
		}else if(thisTurnPlayer==&B){
			LCD_ShowMonster(0,0,(u8 **)bmp_monster,80,100,1);
		}
		// 绘制对手角色
		if(thisTurnEnemy==&A){
			LCD_ShowPika(120,0,(u8 **)bmp_pika,80,100,1);
		}else if(thisTurnEnemy==&B){
			LCD_ShowMonster(120,0,(u8 **)bmp_monster,80,100,1);
		}

		// ability
		if(thisTurnPlayer==&A){
			LCD_ShowString(1,280,200,16,16, A_skill[0].name);
		}else{
			LCD_ShowString(1,280,200,16,16, B_skill[0].name);
		}
		if(thisTurnPlayer==&A){
			LCD_ShowString(61,280,200,16,16, A_skill[1].name);
			}else{
			LCD_ShowString(61,280,200,16,16, B_skill[1].name);
		}
		if(thisTurnPlayer==&A){
			LCD_ShowString(121,280,200,16,16, A_skill[2].name);
		}else{
			LCD_ShowString(121,280,200,16,16, B_skill[2].name);
		}




		sprintf(DATA_TO_SEND, "EXIT");
		LCD_ShowString(181,280,200,16,16, DATA_TO_SEND);


		POINT_COLOR=RED;//设置画笔为红�??
}
void screen_menu3_print(){//第三个界�??

		POINT_COLOR=BLUE;//设置字体为蓝�??
		LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域

		// HP
		if(thisTurnPlayer==&A){
		sprintf(DATA_TO_SEND, "HP: %d",A.HP);
		LCD_ShowString(0,100,200,16,16, DATA_TO_SEND);
		}else{
			sprintf(DATA_TO_SEND, "HP: %d",B.HP);
			LCD_ShowString(0,100,200,16,16, DATA_TO_SEND);
		}

		sprintf(DATA_TO_SEND, "HP: %d",Enemy.HP);
		LCD_ShowString(121,100,200,16,16, DATA_TO_SEND);
		//防御
		if(thisTurnPlayer==&A){
		sprintf(DATA_TO_SEND, "Defense: %d",A.DEF);
		LCD_ShowString(0,120,200,16,16, DATA_TO_SEND);
		}else{
			sprintf(DATA_TO_SEND, "Defense: %d",B.DEF);
			LCD_ShowString(0,120,200,16,16, DATA_TO_SEND);
		}
		sprintf(DATA_TO_SEND, "Defense: %d",Enemy.DEF);
		LCD_ShowString(121,120,200,16,16, DATA_TO_SEND);
		//攻击
		if(thisTurnPlayer==&A){
		sprintf(DATA_TO_SEND, "Attack: %d",A.ATK);
		LCD_ShowString(0,140,200,16,16, DATA_TO_SEND);
		}else{
			sprintf(DATA_TO_SEND, "Attack: %d",B.ATK);
			LCD_ShowString(0,140,200,16,16, DATA_TO_SEND);
		}
		sprintf(DATA_TO_SEND, "Attack: %d",Enemy.ATK);
		LCD_ShowString(121,140,200,16,16, DATA_TO_SEND);
		//名字
		if(thisTurnPlayer==&A){
		sprintf(DATA_TO_SEND, "Name: %s",A.name);
		LCD_ShowString(0,160,200,16,16, DATA_TO_SEND);
		}else{
			sprintf(DATA_TO_SEND, "Name: %s",B.name);
			LCD_ShowString(0,160,200,16,16, DATA_TO_SEND);
		}
		sprintf(DATA_TO_SEND, "Name: %s",Enemy.name);
		LCD_ShowString(121,160,200,16,16, DATA_TO_SEND);
		//法力
		if(thisTurnPlayer==&A){
		sprintf(DATA_TO_SEND, "MP: %d",A.MP);
		LCD_ShowString(0,180,200,16,16, DATA_TO_SEND);
		}else{
			sprintf(DATA_TO_SEND, "MP: %d",B.MP);
			LCD_ShowString(0,180,200,16,16, DATA_TO_SEND);
		}
		sprintf(DATA_TO_SEND, "MP: %d",Enemy.MP);
		LCD_ShowString(121,180,200,16,16, DATA_TO_SEND);


		LCD_ShowString(0, lcddev.height-24, 200, 16, 16, STATE);

		// 绘制游戏界面
		LCD_DrawLine(0,270,240,270);// �??个横�??
		LCD_DrawLine(120,0,120,320);//三个竖线
		LCD_DrawLine(60,270,60,320);
		LCD_DrawLine(180,270,180,320);


		// 绘制角色，图�??
		// 绘制自己角色
		if (thisTurnPlayer==&A){
			LCD_ShowPika(0,0,(u8 **)bmp_pika,80,100,1);
		}else if(thisTurnPlayer==&B){
			LCD_ShowMonster(0,0,(u8 **)bmp_monster,80,100,1);
		}
		// 绘制对手角色
		if(thisTurnEnemy==&A){
			LCD_ShowPika(120,0,(u8 **)bmp_pika,80,100,1);
		}else if(thisTurnEnemy==&B){
			LCD_ShowMonster(120,0,(u8 **)bmp_monster,80,100,1);
		}
		//bag
		LCD_ShowString(1,280,200,16,16, item[0].name);
		LCD_ShowString(61,280,200,16,16, item[1].name);
		LCD_ShowString(121,280,200,16,16, item[2].name);
		sprintf(DATA_TO_SEND, "EXIT");
		LCD_ShowString(181,280,200,16,16, DATA_TO_SEND);
		POINT_COLOR=RED;//设置画笔为红�??
}

void change_state(){
	if(state_num == 0){
		state_num = 1;
		sprintf(STATE, "STATE: ON");
	}else{
		state_num = 0;
		sprintf(STATE, "STATE: OFF");
	}
}
//通信部分
void modeSwitch(void)
{
		if(mode == 0){ //µ±Ç°Îª½ÓÊÜÄ£Ê½
			mode++;
			NRF24L01_TX_Mode(0);


		}else if(mode == 1){ //µ±Ç°Îª·¢ËÍÄ£Ê½
			mode = 0;
			NRF24L01_RX_Mode(0);
		}


	}

void sendData(u8 a, u8 b, u8 c){
	 	key = 0;
		while(1)
		{
			if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
			{
				LCD_ShowString(60,170,239,32,16,"Sended DATA:");
				LCD_ShowString(0,190,239,32,16,tmp_buf);
				key++;


				tmp_buf[0]=a;
				tmp_buf[1]=b;
				tmp_buf[2]=c;

				tmp_buf[32]=0;//¼ÓÈë½áÊø·û
					if(key ==2){ //Ñ­»·Á½´Î·ÀÖ¹Ã»·¢³É¹¦
						modeSwitch();
						break;
					}

			}else
			{
 				LCD_ShowString(60,170,239,32,16,"Send Failed ");
				LCD_Fill(0,188,240,218,WHITE);//Çå¿ÕÉÏÃæµÄÏÔÊ¾
			};
			LED0=!LED0;
			delay_ms(1500);
		};
 }

 void receiveData(void){
	 while(1){
	 if(NRF24L01_RxPacket(tmp_buf)==0)//Ò»µ©½ÓÊÕµ½ÐÅÏ¢,ÔòÏÔÊ¾³öÀ´.
			{
		//receive data code  处理tmp_buf前三位数据

				break;
			}
		}
	 modeSwitch();
 }




//
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	if(x1<size|| x2<size||y1<size|| y2<size)return;
	delta_x=x2-x1; //计算坐标增量
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向
	else if(delta_x==0)incx=0;//垂直�???
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if(delta_y==0)incy=0;//水平�???
	else{incy=-1;delta_y=-delta_y;}
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标�???
	else distance=delta_y;
	for(t=0;t<=distance+1;t++ )//画线输出
	{
		gui_fill_circle(uRow,uCol,size,color);//画点
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
//5个触控点的颜�???(电容触摸屏用)
const u16 POINT_COLOR_TBL[5]={RED,GREEN,BLUE,BROWN,GRED};

//电阻触摸屏测试函�???
void rtp_test(void)
{
	u8 key;
	u8 i=0;
	while(1)
	{
	 	key=KEY_Scan(0);
		tp_dev.scan(0);
		if(menu==1){
			screen_norm_print();
		}else if(menu==2){
			screen_menu2_print();
		}else if(menu==3){
			screen_menu3_print();
		}

		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16){
					screen_print();//清除
				}
				else if(tp_dev.x[0]>0&&tp_dev.y[0]>270&&tp_dev.x[0]<60&&tp_dev.y[0]<lcddev.height){
					// �??1的按�??
					if(menu==1){
						LCD_Clear(WHITE);//清屏
						screen_menu2_print();//到界�??2
						menu=2;
						getInstruction = 0; //
					}else if(menu==2){
						//�??1�??�??
						int tempval;
						if(thisTurnPlayer==&A){
							struct skill *Sp;
							Sp = &A_skill[0];
							if (judgeMP(thisTurnPlayer, Sp)){
								tempval = attackSkill(thisTurnPlayer, Sp);//将该值用通信传出
								beAttacked(thisTurnEnemy, tempval);
								normailzeATKValue(thisTurnPlayer);

								printCharaterState();
								getInstruction = -1;
								round++;
								menu=1;
								LCD_Clear(WHITE);//清屏
							}
						}else{
							struct skill *Sp;
							Sp = &B_skill[0];
							if (judgeMP(thisTurnPlayer, Sp)){
								tempval = attackSkill(thisTurnPlayer, Sp);//将该值用通信传出
								beAttacked(thisTurnEnemy, tempval);
								normailzeATKValue(thisTurnPlayer);

								printCharaterState();
								getInstruction = -1;
								round++;
								menu=1;
								LCD_Clear(WHITE);//清屏
							}
						}

						sprintf(DATA_TO_SEND, "using ability1");
						HAL_UART_Transmit(&huart1, (uint8_t*)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);

					}else if(menu==3){
						struct item *Ip;
						Ip = &item[0];
						if(judgeCount(Ip)){
							useItem(thisTurnPlayer,Ip);

							printCharaterState();
							getInstruction = -1;
							round++;
							menu=1;
							LCD_Clear(WHITE);//清屏
						}
						sprintf(DATA_TO_SEND, "using item1");
						HAL_UART_Transmit(&huart1, (uint8_t*)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);
					}
				}
				else if(tp_dev.x[0]>60&&tp_dev.y[0]>270&&tp_dev.x[0]<120&&tp_dev.y[0]<lcddev.height){
					// �??2的按�??
					if(menu==1){
						// 换人
						sprintf(DATA_TO_SEND, "exchange");
						HAL_UART_Transmit(&huart1, (uint8_t*)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);
						getInstruction = 2;
						changeCharacter();
						LCD_Clear(WHITE);//清屏
					}else if(menu==2){
						if(thisTurnPlayer==&A){
							struct skill *Sp;
							Sp = &A_skill[1];
							if (judgeMP(thisTurnPlayer, Sp)){
								int tempval;
								tempval = attackSkill(thisTurnPlayer, Sp);//将该值用通信传出
								beAttacked(thisTurnEnemy, tempval);
								normailzeATKValue(thisTurnPlayer);

								printCharaterState();
								getInstruction = -1;
								round++;
								menu=1;
								LCD_Clear(WHITE);//清屏
							}
						}else{
							struct skill *Sp;
							Sp = &B_skill[1];
							if (judgeMP(thisTurnPlayer, Sp)){
								int tempval;
								tempval = attackSkill(thisTurnPlayer, Sp);//将该值用通信传出
								beAttacked(thisTurnEnemy, tempval);
								normailzeATKValue(thisTurnPlayer);

								printCharaterState();
								getInstruction = -1;
								round++;
								menu=1;
								LCD_Clear(WHITE);//清屏
							}
						}


						sprintf(DATA_TO_SEND, "using ability2");
						HAL_UART_Transmit(&huart1, (uint8_t*)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);

					}else if(menu==3){

						struct item *Ip;
						Ip = &item[1];
						if(judgeCount(Ip)){
							useItem(thisTurnPlayer,Ip);

							printCharaterState();
							getInstruction = -1;
							round++;
							menu=1;
							LCD_Clear(WHITE);//清屏
						}
						sprintf(DATA_TO_SEND, "using item2");
						HAL_UART_Transmit(&huart1, (uint8_t*)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);

					}

				}else if(tp_dev.x[0]>120&&tp_dev.y[0]>270&&tp_dev.x[0]<180&&tp_dev.y[0]<lcddev.height){

					if(menu==1){
						getInstruction = 3;
						sprintf(DATA_TO_SEND, "escape");
						HAL_UART_Transmit(&huart1, (uint8_t*)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);
						LCD_Clear(WHITE);//清屏
					}else if(menu==2){
						if(thisTurnPlayer==&A){
							struct skill *Sp;
							Sp = &A_skill[2];
							if (judgeMP(thisTurnPlayer, Sp)){
								int tempval;
								tempval = attackSkill(thisTurnPlayer, Sp);//将该值用通信传出
								beAttacked(thisTurnEnemy, tempval);
								normailzeATKValue(thisTurnPlayer);

								printCharaterState();
								getInstruction = -1;
								round++;
								menu=1;
								LCD_Clear(WHITE);//清屏
							}
						}else{
							struct skill *Sp;
							Sp = &B_skill[2];
							if (judgeMP(thisTurnPlayer, Sp)){
								int tempval;
								tempval = attackSkill(thisTurnPlayer, Sp);//将该值用通信传出
								beAttacked(thisTurnEnemy, tempval);
								normailzeATKValue(thisTurnPlayer);

								printCharaterState();
								getInstruction = -1;
								round++;

								menu=1;
								LCD_Clear(WHITE);//清屏
							}
						}
						sprintf(DATA_TO_SEND, "using ability3");
						HAL_UART_Transmit(&huart1, (uint8_t*)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);

					}else if(menu==3){
						struct item *Ip;
						Ip = &item[2];
						if(judgeCount(Ip)){
							useItem(thisTurnPlayer,Ip);

							printCharaterState();
							getInstruction = -1;
							round++;
							menu=1;
							LCD_Clear(WHITE);//清屏
						}
						sprintf(DATA_TO_SEND, "using item3");
						HAL_UART_Transmit(&huart1, (uint8_t*)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);

					}
				}
				else if(tp_dev.x[0]>180&&tp_dev.y[0]>270&&tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height){
					// �??右边的按�??
					if(menu==2){
						sprintf(DATA_TO_SEND, "EXIT1");
						HAL_UART_Transmit(&huart1, (uint8_t*)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);
						screen_print();
						menu=1;
					}else if(menu==1){
						LCD_Clear(WHITE);//清屏
						screen_menu3_print();
						menu=3;
					}else if(menu==3){
						sprintf(DATA_TO_SEND, "EXIT2");
						HAL_UART_Transmit(&huart1, (uint8_t*)DATA_TO_SEND, strlen(DATA_TO_SEND), HAL_MAX_DELAY);
						screen_print();
						menu=1;
					}

				}
				else if(tp_dev.x[0]>0&&tp_dev.y[0]>lcddev.height-24&&tp_dev.x[0]<80&&tp_dev.y[0]<lcddev.height){
					change_state();
				}
				else {
					TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);//画图
				}
			}
		}else delay_ms(20);	//没有按键按下的时�??? 本来�??10
		if(key==KEY0_PRES)	//KEY0按下,则执行校准程�???
		{

//			LCD_Clear(WHITE);	//清屏
//		    TP_Adjust();  		//屏幕校准
//			TP_Save_Adjdata();
//			Load_Drow_Dialog();
		}
		i++;
		// if(i%20==0)LED0=!LED0;
	}
}
//电容触摸屏测试函�???
void ctp_test(void)
{
	u8 t=0;
	u8 i=0;
 	u16 lastpos[5][2];		//�???后一次的数据
	while(1)
	{
		tp_dev.scan(0);
		for(t=0;t<5;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
                //printf("X坐标:%d,Y坐标:%d\r\n",tp_dev.x[0],tp_dev.y[0]);
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}

					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<20)
					{
						Load_Drow_Dialog();//清除
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}

		delay_ms(5);i++;
		// if(i%20==0)LED0=!LED0;
	}
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函�???
//	uart_init(115200);					//初始化串�???
//	usmart_dev.init(84); 		  	  	//初始化USMART
	LED_Init();							//初始化LED
	KEY_Init();							//初始化按�???
	LCD_Init();							//初始化LCD
	tp_dev.init();				   		//触摸屏初始化
	NRF24L01_Init();    	//初始化³õÊ¼»¯NRF24L01
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  //加载两个人物
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_UART_Receive_IT(&huart1, (uint8_t* )rxBuffer,1);

  unsigned char name1[20];
  sprintf(name1, "Pika");
  A = newCharacter(name1,4,100, 100, 40, 40);
  sprintf(name1, "Mon");
  B = newCharacter(name1,3,100, 100, 40, 40);

  sprintf(name1, "GG");
  Enemy = newCharacter(name1,2,9999, 9999, 9999, 9999);
  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);


  unsigned char names[20];
  unsigned char *p;
  sprintf(names, "Collision");
  p = names;
  A_skill[0] = newSkill(p, 9, 20, 10, 0);
  sprintf(names, "Heal");
  p = names;
  A_skill[1] = newSkill(p, 4, 20, 15, 5);
  sprintf(names, "Brave");
  p = names;
  A_skill[2] = newSkill(p, 5, 10, 8, 2);
  sprintf(names, "Threaten");
  p = names;
  A_skill[3] = newSkill(p, 8, 10, 8, 4);

  sprintf(names, "Jerk");
  p = names;
  B_skill[0] = newSkill(p, 4, 20, 6, 0);
  sprintf(names, "Sleep");
  p = names;
  B_skill[1] = newSkill(p, 5, 20, 15, 5);
  sprintf(names, "Boki");
  p = names;
  B_skill[2] = newSkill(p, 4, 10, 14, 2);
  sprintf(names, "Ahhh");
  p = names;
  B_skill[3] = newSkill(p, 4, 10, 10, 4);

  sprintf(names, "Add_HP");
  p = names;
  item[0] = newItem(p, 6, 30, 3, 0);

  sprintf(names, "Add_MP");
  p = names;
  item[1] = newItem(p, 6, 30, 3, 1);

  sprintf(names, "Add_ATK");
  p = names;
  item[2] = newItem(p, 7, 2, 3, 2);

  sprintf(names, "Add_DEF");
  p = names;
  item[3] = newItem(p, 7, 2, 3, 3);


  POINT_COLOR=RED;
  	LCD_ShowString(30,50,200,16,16,"Mini STM32");
  	LCD_ShowString(30,70,200,16,16,"TOUCH TEST");
  	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
  	LCD_ShowString(30,110,200,16,16,"2019/11/15");
	while(NRF24L01_Check())	//¼ì²éNRF24L01ÊÇ·ñÔÚÎ».
	{
		LCD_ShowString(60,130,200,16,16,"NRF24L01 Error");
		delay_ms(200);
		LCD_Fill(60,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(60,130,200,16,16,"NRF24L01 OK");
	while(1)//ÔÚ¸Ã²¿·ÖÈ·¶¨½øÈëÄÄ¸öÄ£Ê½!
		{
			key=KEY_Scan(0);
			if(key==KEY0_PRES)
			{
				mode=0;
				break;
			}else if(key==KEY1_PRES)
			{
				mode=1;
				break;
			}
			t++;
			if(t==100)LCD_ShowString(10,150,230,16,16,"KEY0:RX_Mode  KEY1:TX_Mode"); //ÉÁË¸ÏÔÊ¾ÌáÊ¾ÐÅÏ¢
	 		if(t==200)
			{
				LCD_Fill(10,150,230,150+16,WHITE);
				t=0;
			}
			delay_ms(5);
		}
	 	LCD_Fill(10,150,240,166,WHITE);//Çå¿ÕÉÏÃæµÄÏÔÊ¾
	 	POINT_COLOR=BLUE;//ÉèÖÃ×ÖÌåÎªÀ¶É«
		if(mode==0)//RXÄ£Ê½
		{
			LCD_ShowString(60,150,200,16,16,"NRF24L01 RX_Mode");
	//LCD_ShowString(60,170,200,16,16,"Received DATA:");
			NRF24L01_RX_Mode(0);
	//		while(1)
	//		{
	//			if(NRF24L01_RxPacket(tmp_buf)==0)//Ò»µ©½ÓÊÕµ½ÐÅÏ¢,ÔòÏÔÊ¾³öÀ´.
	//			{
	//				tmp_buf[32]=0;//¼ÓÈë×Ö·û´®½áÊø·û
	//				LCD_ShowString(0,190,239,32,16,tmp_buf);
	//			}else delay_us(100);
	//			t++;
	//			if(t==10000)//´óÔ¼1sÖÓ¸Ä±äÒ»´Î×´Ì¬
	//			{
	//				t=0;
	//				LED0=!LED0;
	//			}
	//		};

			//接收端
			LCD_Clear(WHITE);//清屏
			rtp_test();

		}else//TXÄ£Ê½
		{
			LCD_ShowString(60,150,200,16,16,"NRF24L01 TX_Mode");
			NRF24L01_TX_Mode(0);
			//发送
			LCD_Clear(WHITE);//清屏
			rtp_test();
		}


   	Load_Drow_Dialog();

//  	if(tp_dev.touchtype&0X80)ctp_test();//电容�??
//  	else rtp_test(); 					//电阻�??
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7199;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED0_Pin */
  GPIO_InitStruct.Pin = LED0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

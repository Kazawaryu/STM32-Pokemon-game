#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32F103������
//2.4��/2.8��/3.5��/4.3��/7�� TFTҺ������
//֧������IC�ͺŰ���:ILI9341/NT35310/NT35510/SSD1963/ST7789 ��
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/7/4
//�汾��V3.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//V4.0 20211110
//1��ȥ���󲿷ֲ���������IC��֧��
//2��������ST7789����IC��֧��
//3���Ż�����ṹ������Դ�룩
//////////////////////////////////////////////////////////////////////////////////


//LCD��Ҫ������
typedef struct  
{
    u16 width;      //LCD ���
    u16 height;     //LCD �߶�
    u16 id;         //LCD ID
    u8  dir;        //���������������ƣ�0��������1��������
    u16 wramcmd;    //��ʼдgramָ��
    u16 setxcmd;    //����x����ָ��
    u16  setycmd;   //����y����ָ��
} _lcd_dev;

//LCD����
extern _lcd_dev lcddev; //����LCD��Ҫ����
//LCD�Ļ�����ɫ�ͱ���ɫ
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 
#define LCD_LED PCout(10)               //LCD����       PC10
 
#define LCD_CS_SET  GPIOC->BSRR=1<<9    //Ƭѡ�˿�      PC9
#define LCD_RS_SET  GPIOC->BSRR=1<<8    //����/����     PC8
#define LCD_WR_SET  GPIOC->BSRR=1<<7    //д����        PC7
#define LCD_RD_SET  GPIOC->BSRR=1<<6    //������        PC6

#define LCD_CS_CLR  GPIOC->BRR=1<<9     //Ƭѡ�˿�      PC9
#define LCD_RS_CLR  GPIOC->BRR=1<<8     //����/����     PC8
#define LCD_WR_CLR  GPIOC->BRR=1<<7     //д����        PC7
#define LCD_RD_CLR  GPIOC->BRR=1<<6     //������        PC6

//PB0~15,��Ϊ������
#define DATAOUT(x) GPIOB->ODR=x; //�������
#define DATAIN     GPIOB->IDR;   //��������

 
//////////////////////////////////////////////////////////////////////
//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//������ɫ
#define WHITE           0xFFFF
#define BLACK           0x0000
#define BLUE            0x001F
#define BRED            0XF81F
#define GRED            0XFFE0
#define GBLUE           0X07FF
#define RED             0xF800
#define MAGENTA         0xF81F
#define GREEN           0x07E0
#define CYAN            0x7FFF
#define YELLOW          0xFFE0
#define BROWN           0XBC40  //��ɫ
#define BRRED           0XFC07  //�غ�ɫ
#define GRAY            0X8430  //��ɫ
//GUI��ɫ

#define DARKBLUE        0X01CF  //����ɫ
#define LIGHTBLUE       0X7D7C  //ǳ��ɫ  
#define GRAYBLUE        0X5458  //����ɫ
//������ɫΪPANEL����ɫ

#define LIGHTGREEN      0X841F  //ǳ��ɫ 
#define LGRAY           0XC618  //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE       0XA651  //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0X2B12  //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


void LCD_Init(void);                                                        //��ʼ��
void LCD_DisplayOn(void);                                                   //����ʾ
void LCD_DisplayOff(void);                                                  //����ʾ
void LCD_Clear(u16 Color);                                                  //����
void LCD_SetCursor(u16 Xpos, u16 Ypos);                                     //���ù��
void LCD_DrawPoint(u16 x, u16 y);                                           //����
void LCD_Fast_DrawPoint(u16 x, u16 y, u16 color);                           //���ٻ���
u16  LCD_ReadPoint(u16 x, u16 y);                                           //����
void LCD_Draw_Circle(u16 x0, u16 y0, u8 r);                                 //��Բ
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);                          //����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);                     //������
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);                   //��䵥ɫ
void LCD_Color_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 *color);            //���ָ����ɫ
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode);                  //��ʾһ���ַ�
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size);                   //��ʾһ������
void LCD_ShowxNum(u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode);         //��ʾ ����
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p);   //��ʾһ���ַ���,12/16����

void LCD_ShowBMP(u16 x,u16 y,unsigned char** bmp,u8 bmp_height,u8 row,u8 mode);

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_SSD_BackLightSet(u8 pwm);                          //SSD1963 �������
void LCD_Scan_Dir(u8 dir);                                  //������ɨ�跽��
void LCD_Display_Dir(u8 dir);                               //������Ļ��ʾ����
void LCD_Set_Window(u16 sx, u16 sy, u16 width, u16 height); //���ô���

//д���ݺ���
#define LCD_WR_DATA(data){\
        LCD_RS_SET;\
        LCD_CS_CLR;\
        DATAOUT(data);\
        LCD_WR_CLR;\
        LCD_WR_SET;\
        LCD_CS_SET;\
    }

//LCD�ֱ�������
#define SSD_HOR_RESOLUTION      800     //LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION      480     //LCD��ֱ�ֱ���
//LCD������������
#define SSD_HOR_PULSE_WIDTH     1       //ˮƽ����
#define SSD_HOR_BACK_PORCH      46      //ˮƽǰ��
#define SSD_HOR_FRONT_PORCH     210     //ˮƽ����

#define SSD_VER_PULSE_WIDTH     1       //��ֱ����
#define SSD_VER_BACK_PORCH      23      //��ֱǰ��
#define SSD_VER_FRONT_PORCH     22      //��ֱǰ��
//���¼����������Զ�����
#define SSD_HT  (SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS (SSD_HOR_BACK_PORCH)
#define SSD_VT  (SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif














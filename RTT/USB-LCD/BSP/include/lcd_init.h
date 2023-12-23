#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "stm32f1xx_hal.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

// ���ʹ��FATFS���˺궨����Ч
#define USE_HORIZONTAL 1  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����


#define LCD_W 240
#define LCD_H 240


#define DATAOUT(x)	{GPIOA->BSRR=((x&0x0000000F)|0x000F0000); \
						GPIOB->BSRR=(((x&0x000000F0)<<4)|0x0F000000);} //�������
#define DATAIN     ((((GPIOB->IDR)&0x0F00)>>4)|((GPIOA->IDR)&0x000F));   //��������

#define	LCD_RES_Set()	GPIOC->BSRR=1<<13    //��λ			    PC13
#define	LCD_CS_Set()  GPIOA->BSRR=1<<8    //Ƭѡ�˿�    	PA8
#define	LCD_DC_Set()  GPIOB->BSRR=1<<4    //����/����     PB4
#define	LCD_WR_Set()	GPIOB->BSRR=1<<5    //д����	      PB5
#define	LCD_RD_Set()	GPIOB->BSRR=1<<3    //������		    PB3
#define	LCD_BLK_Set()	GPIOA->BSRR=1<<15    //�������		  PA15

#define	LCD_RES_Clr()	GPIOC->BRR=1<<13     //��λ				PC13
#define	LCD_CS_Clr()  GPIOA->BRR=1<<8    //Ƭѡ�˿�    PA8
#define	LCD_DC_Clr()	GPIOB->BRR=1<<4     //����/����   PB4
#define	LCD_WR_Clr()	GPIOB->BRR=1<<5     //д����	    PB5
#define	LCD_RD_Clr()	GPIOB->BRR=1<<3     //������	    PB3
#define	LCD_BLK_Clr()	GPIOA->BRR=1<<15     //�������    PA15



void LCD_WR_DATA8(u8 dat);//д��һ���ֽ�
void LCD_WR_DATA8_Faster(u8* dat, u32 size);
void LCD_WR_DATA(u16 dat);//д�������ֽ�
void LCD_WR_DATA_Faster(u16* dat, u32 size);
void LCD_WR_REG(u8 dat);//д��һ��ָ��
void LCD_Area_Set(u16 x1,u16 y1,u16 x2,u16 y2);//������ʾ����
void LCD_Init(void);//LCD��ʼ��
#endif





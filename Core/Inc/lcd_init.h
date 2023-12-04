#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

// 如果使用FATFS，此宏定义无效
#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#define LCD_W 240
#define LCD_H 240


#define DATAOUT(x)	{GPIOA->BSRR=((x&0x0000000F)|0x000F0000); \
						GPIOB->BSRR=(((x&0x000000F0)<<4)|0x0F000000);} //数据输出
#define DATAIN     ((((GPIOB->IDR)&0x0F00)>>4)|((GPIOA->IDR)&0x000F));   //数据输入

#define	LCD_RES_Set()	GPIOC->BSRR=1<<13    //复位			    PC13
#define	LCD_CS_Set()  GPIOA->BSRR=1<<8    //片选端口    	PA8
#define	LCD_DC_Set()  GPIOB->BSRR=1<<4    //数据/命令     PB4
#define	LCD_WR_Set()	GPIOB->BSRR=1<<5    //写数据	      PB5
#define	LCD_RD_Set()	GPIOB->BSRR=1<<3    //读数据		    PB3
#define	LCD_BLK_Set()	GPIOA->BSRR=1<<15    //背光控制		  PA15

#define	LCD_RES_Clr()	GPIOC->BRR=1<<13     //复位				PC13
#define	LCD_CS_Clr()  GPIOA->BRR=1<<8    //片选端口    PA8
#define	LCD_DC_Clr()	GPIOB->BRR=1<<4     //数据/命令   PB4
#define	LCD_WR_Clr()	GPIOB->BRR=1<<5     //写数据	    PB5
#define	LCD_RD_Clr()	GPIOB->BRR=1<<3     //读数据	    PB3
#define	LCD_BLK_Clr()	GPIOA->BRR=1<<15     //背光控制    PA15



void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA8_Faster(u8* dat, u32 size);
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_DATA_Faster(u16* dat, u32 size);
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Area_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置显示区域
void LCD_Init(void);//LCD初始化
#endif





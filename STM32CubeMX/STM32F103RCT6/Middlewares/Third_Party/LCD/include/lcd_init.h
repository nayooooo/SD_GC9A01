#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "main.h"

// 如果使用FATFS，此宏定义无效
#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#define LCD_W 240
#define LCD_H 240


#define DATAOUT(x)  GPIOC->BSRR=(((~x)&0x000000FF)<<16)|x
//#define DATAOUT(x)  {GPIOC->BSRR=((~x)<<16)|x}
#define DATAIN      ((GPIOC->IDR)&0xFF)

#define	LCD_RES_Set()	LCD_RES_GPIO_Port->BSRR=LCD_RES_Pin
#define	LCD_CS_Set()    LCD_CS_GPIO_Port->BSRR=LCD_CS_Pin
#define	LCD_DC_Set()    LCD_DC_GPIO_Port->BSRR=LCD_DC_Pin
#define	LCD_WR_Set()	LCD_WR_GPIO_Port->BSRR=LCD_WR_Pin
#define	LCD_RD_Set()	LCD_RD_GPIO_Port->BSRR=LCD_RD_Pin
#define	LCD_BLK_Set()	LCD_BLK_GPIO_Port->BSRR=LCD_BLK_Pin

#define	LCD_RES_Clr()	LCD_RES_GPIO_Port->BRR=LCD_RES_Pin
#define	LCD_CS_Clr()    LCD_CS_GPIO_Port->BRR=LCD_CS_Pin
#define	LCD_DC_Clr()	LCD_DC_GPIO_Port->BRR=LCD_DC_Pin
#define	LCD_WR_Clr()	LCD_WR_GPIO_Port->BRR=LCD_WR_Pin
#define	LCD_RD_Clr()	LCD_RD_GPIO_Port->BRR=LCD_RD_Pin
#define	LCD_BLK_Clr()	LCD_BLK_GPIO_Port->BRR=LCD_BLK_Pin



void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA8_Faster(u8* dat, u32 size);
void LCD_WR_DATA8_Faster_LVGL(u8* dat, u32 size);
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_DATA_Faster(u16* dat, u32 size);
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Area_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置显示区域
void LCD_Init(void);//LCD初始化
#endif





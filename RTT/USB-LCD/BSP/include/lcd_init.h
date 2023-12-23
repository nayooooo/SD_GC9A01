#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "stm32f1xx_hal.h"

#include "./cubemx/inc/main.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

// ���ʹ��FATFS���˺궨����Ч
#define USE_HORIZONTAL 1  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����


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



void LCD_WR_DATA8(u8 dat);//д��һ���ֽ�
void LCD_WR_DATA8_Faster(u8* dat, u32 size);
void LCD_WR_DATA(u16 dat);//д�������ֽ�
void LCD_WR_DATA_Faster(u16* dat, u32 size);
void LCD_WR_REG(u8 dat);//д��һ��ָ��
void LCD_Area_Set(u16 x1,u16 y1,u16 x2,u16 y2);//������ʾ����
void LCD_Init(void);//LCD��ʼ��
#endif





#include "lcd_init.h"

#if USE_FATFS
#include "fatfs.h"
#include <stdio.h>
#include <string.h>
#endif

static void delay_ms(u32 nms)
{
	HAL_Delay(nms);
}

static void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

      /* GPIO Ports Clock Enable */
      __HAL_RCC_GPIOC_CLK_ENABLE();
      __HAL_RCC_GPIOD_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();
      __HAL_RCC_GPIOB_CLK_ENABLE();

      /*Configure GPIO pin Output Level */
      HAL_GPIO_WritePin(GPIOC, LCD_DB0_Pin|LCD_DB1_Pin|LCD_DB2_Pin|LCD_DB3_Pin
                              |LCD_DB4_Pin|LCD_DB5_Pin|LCD_DB6_Pin|LCD_DB7_Pin, GPIO_PIN_SET);

      /*Configure GPIO pin Output Level */
      HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET);

      /*Configure GPIO pin Output Level */
      HAL_GPIO_WritePin(GPIOB, LCD_BLK_Pin|LCD_CS_Pin|LCD_DC_Pin|LCD_WR_Pin
                              |LCD_RD_Pin, GPIO_PIN_SET);

      /*Configure GPIO pins : LCD_DB0_Pin LCD_DB1_Pin LCD_DB2_Pin LCD_DB3_Pin
                               LCD_DB4_Pin LCD_DB5_Pin LCD_DB6_Pin LCD_DB7_Pin */
      GPIO_InitStruct.Pin = LCD_DB0_Pin|LCD_DB1_Pin|LCD_DB2_Pin|LCD_DB3_Pin
                              |LCD_DB4_Pin|LCD_DB5_Pin|LCD_DB6_Pin|LCD_DB7_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      /*Configure GPIO pin : LCD_RES_Pin */
      GPIO_InitStruct.Pin = LCD_RES_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(LCD_RES_GPIO_Port, &GPIO_InitStruct);

      /*Configure GPIO pin : LCD_BLK_Pin */
      GPIO_InitStruct.Pin = LCD_BLK_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(LCD_BLK_GPIO_Port, &GPIO_InitStruct);

      /*Configure GPIO pins : LCD_CS_Pin LCD_DC_Pin LCD_WR_Pin LCD_RD_Pin */
      GPIO_InitStruct.Pin = LCD_CS_Pin|LCD_DC_Pin|LCD_WR_Pin|LCD_RD_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
static void LCD_Writ_Bus(u8 dat) 
{	
	DATAOUT(dat);
	LCD_WR_Clr();
	LCD_WR_Set();
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_DC_Set();
	LCD_CS_Clr();
	LCD_Writ_Bus(dat);
	LCD_CS_Set();
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8_Faster(u8* dat, u32 size)
{
	LCD_DC_Set();
	LCD_CS_Clr();
	for (u32 i = 0; i < size; i++) {
		LCD_Writ_Bus(dat[i]);
	}
	LCD_CS_Set();
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	LCD_DC_Set();
	LCD_CS_Clr();
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
	LCD_CS_Set();
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA_Faster(u16* dat, u32 size)
{
	LCD_DC_Set();
	LCD_CS_Clr();
	for (u32 i = 0; i < size; i++) {
		LCD_Writ_Bus(dat[i]>>8);
		LCD_Writ_Bus(dat[i]);
	}
	LCD_CS_Set();
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//д����
	LCD_CS_Clr();
	LCD_Writ_Bus(dat);
	LCD_CS_Set();
}

/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Area_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
}

void LCD_Init(void)
{
	LCD_GPIO_Init();//��ʼ��GPIO
	
	LCD_RES_Clr();//��λ
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);
	
	LCD_BLK_Set();//�򿪱���
  delay_ms(100);
	
	LCD_WR_REG(0xEF);
	LCD_WR_REG(0xEB);
	LCD_WR_DATA8(0x14); 
	
  LCD_WR_REG(0xFE);			 
	LCD_WR_REG(0xEF); 

	LCD_WR_REG(0xEB);	
	LCD_WR_DATA8(0x14); 

	LCD_WR_REG(0x84);			
	LCD_WR_DATA8(0x40); 

	LCD_WR_REG(0x85);			
	LCD_WR_DATA8(0xFF); 

	LCD_WR_REG(0x86);			
	LCD_WR_DATA8(0xFF); 

	LCD_WR_REG(0x87);			
	LCD_WR_DATA8(0xFF);

	LCD_WR_REG(0x88);			
	LCD_WR_DATA8(0x0A);

	LCD_WR_REG(0x89);			
	LCD_WR_DATA8(0x21); 

	LCD_WR_REG(0x8A);			
	LCD_WR_DATA8(0x00); 

	LCD_WR_REG(0x8B);			
	LCD_WR_DATA8(0x80); 

	LCD_WR_REG(0x8C);			
	LCD_WR_DATA8(0x01); 

	LCD_WR_REG(0x8D);			
	LCD_WR_DATA8(0x01); 

	LCD_WR_REG(0x8E);			
	LCD_WR_DATA8(0xFF); 

	LCD_WR_REG(0x8F);			
	LCD_WR_DATA8(0xFF); 


	LCD_WR_REG(0xB6);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x20);

	LCD_WR_REG(0x36);
#if USE_FATFS
	FATFS fs;
	FIL fil;
	FRESULT fres = f_mount(&fs, "sd", 1);
	if (fres != FR_OK) {
		printf("[LCD] FatFS mount sd card failed!\r\n");
		goto lcd_fatfs_config_error;
	}
	if (f_open(&fil, "USBSD_GC9A01/Config/gc9a01.conf", FA_READ) != FR_OK) {
		printf("[LCD] open file(gc9a01.conf) failed!\r\n");
		goto lcd_fatfs_config_error;
	}
	char buff[20] = { 0 };
	UINT br;
	f_read(&fil, buff, 20, &br);
	char* pos = strstr(buff, "HORIZONTAL");
	pos = strstr(pos, "="); pos++;
	int lcd_horizontal;
	sscanf(pos, "%d", &lcd_horizontal);
	if (lcd_horizontal==0)LCD_WR_DATA8(0x08);
	else if(lcd_horizontal==1)LCD_WR_DATA8(0xC8);
	else if(lcd_horizontal==2)LCD_WR_DATA8(0x68);
	else LCD_WR_DATA8(0xA8);
	if (0) {
	lcd_fatfs_config_error:
		LCD_WR_DATA8(0x08);
	}
	f_close(&fil);
	f_mount(&fs, "sd", 0);
#else
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x68);
	else LCD_WR_DATA8(0xA8);
#endif

	LCD_WR_REG(0x3A);			
	LCD_WR_DATA8(0x05); 


	LCD_WR_REG(0x90);			
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08); 

	LCD_WR_REG(0xBD);			
	LCD_WR_DATA8(0x06);
	
	LCD_WR_REG(0xBC);			
	LCD_WR_DATA8(0x00);	

	LCD_WR_REG(0xFF);			
	LCD_WR_DATA8(0x60);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);

	LCD_WR_REG(0xC3);			
	LCD_WR_DATA8(0x13);
	LCD_WR_REG(0xC4);			
	LCD_WR_DATA8(0x13);

	LCD_WR_REG(0xC9);			
	LCD_WR_DATA8(0x22);

	LCD_WR_REG(0xBE);			
	LCD_WR_DATA8(0x11); 

	LCD_WR_REG(0xE1);			
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x0E);

	LCD_WR_REG(0xDF);			
	LCD_WR_DATA8(0x21);
	LCD_WR_DATA8(0x0c);
	LCD_WR_DATA8(0x02);

	LCD_WR_REG(0xF0);   
	LCD_WR_DATA8(0x45);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x26);
 	LCD_WR_DATA8(0x2A);

 	LCD_WR_REG(0xF1);    
 	LCD_WR_DATA8(0x43);
 	LCD_WR_DATA8(0x70);
 	LCD_WR_DATA8(0x72);
 	LCD_WR_DATA8(0x36);
 	LCD_WR_DATA8(0x37);  
 	LCD_WR_DATA8(0x6F);


 	LCD_WR_REG(0xF2);   
 	LCD_WR_DATA8(0x45);
 	LCD_WR_DATA8(0x09);
 	LCD_WR_DATA8(0x08);
 	LCD_WR_DATA8(0x08);
 	LCD_WR_DATA8(0x26);
 	LCD_WR_DATA8(0x2A);

 	LCD_WR_REG(0xF3);   
 	LCD_WR_DATA8(0x43);
 	LCD_WR_DATA8(0x70);
 	LCD_WR_DATA8(0x72);
 	LCD_WR_DATA8(0x36);
 	LCD_WR_DATA8(0x37); 
 	LCD_WR_DATA8(0x6F);

	LCD_WR_REG(0xED);	
	LCD_WR_DATA8(0x1B); 
	LCD_WR_DATA8(0x0B); 

	LCD_WR_REG(0xAE);			
	LCD_WR_DATA8(0x77);
	
	LCD_WR_REG(0xCD);			
	LCD_WR_DATA8(0x63);		


	LCD_WR_REG(0x70);			
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0E); 
	LCD_WR_DATA8(0x0F); 
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x03);

	LCD_WR_REG(0xE8);			
	LCD_WR_DATA8(0x34);

	LCD_WR_REG(0x62);			
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xED);
	LCD_WR_DATA8(0x70); 
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xEF);
	LCD_WR_DATA8(0x70); 
	LCD_WR_DATA8(0x70);

	LCD_WR_REG(0x63);			
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xF1);
	LCD_WR_DATA8(0x70); 
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xF3);
	LCD_WR_DATA8(0x70); 
	LCD_WR_DATA8(0x70);

	LCD_WR_REG(0x64);			
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x29);
	LCD_WR_DATA8(0xF1);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0xF1);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x07);

	LCD_WR_REG(0x66);			
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xCD);
	LCD_WR_DATA8(0x67);
	LCD_WR_DATA8(0x45);
	LCD_WR_DATA8(0x45);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);

	LCD_WR_REG(0x67);			
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x32);
	LCD_WR_DATA8(0x98);

	LCD_WR_REG(0x74);			
	LCD_WR_DATA8(0x10);	
	LCD_WR_DATA8(0x85);	
	LCD_WR_DATA8(0x80);
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x00); 
	LCD_WR_DATA8(0x4E);
	LCD_WR_DATA8(0x00);					
	
  LCD_WR_REG(0x98);			
	LCD_WR_DATA8(0x3e);
	LCD_WR_DATA8(0x07);

	LCD_WR_REG(0x35);	
	LCD_WR_REG(0x21);

	LCD_WR_REG(0x11);
	delay_ms(120);
	LCD_WR_REG(0x29);
	delay_ms(20);
} 








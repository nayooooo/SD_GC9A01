#include "CTIIC.h"

void CTP_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = CTP_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(CTP_SDA_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(CTP_SDA_GPIO_Port, CTP_SDA_Pin, GPIO_PIN_RESET);
}

void CTP_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = CTP_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(CTP_SDA_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(CTP_SDA_GPIO_Port, CTP_SDA_Pin, GPIO_PIN_SET);
}



void CTP_GPIOInit(void)
{
	;
}

void CTP_INT_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = CTP_INT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(CTP_INT_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(CTP_INT_GPIO_Port, CTP_INT_Pin, GPIO_PIN_RESET);
}

//控制I2C速度的延时
void CT_Delay(void)
{
	delay_us(10);
} 

void CTP_IIC_Start(void)
{
	CTP_SDA_OUT();
	CTP_SDA_Set();
	CTP_SCL_Set();
	delay_us(10);
	CTP_SDA_Clr();
	delay_us(5);
	CTP_SCL_Clr();
}
void CTP_IIC_Stop(void)
{
	CTP_SDA_OUT();
	CTP_SCL_Clr();
	CTP_SDA_Clr();
	delay_us(5);
	CTP_SCL_Set();
	CTP_SDA_Set();
}


u8 CTP_WaitAck(void)
{
	u8 ms=0;
	CTP_SDA_IN();
	CTP_SDA_Set();
	CTP_SCL_Set();
	delay_us(5);
	while(CTP_ReadSDA())
	{
		ms++;
		if(ms>250)
		{
			CTP_IIC_Stop();
			return 1;
		}
	}
	CTP_SCL_Clr();
	return 0;
}

void CTP_IICAck(void)
{
	CTP_SCL_Clr();
	CTP_SDA_OUT();
	delay_us(5);
	CTP_SDA_Clr();
	delay_us(5);
	CTP_SCL_Set();
	delay_us(5);
	CTP_SCL_Clr();
	
}

void CTP_IICNack(void)
{
	CTP_SCL_Clr();
	CTP_SDA_OUT();
	delay_us(5);
	CTP_SDA_Set();
	delay_us(5);
	CTP_SCL_Set();
	delay_us(5);
	CTP_SCL_Clr();
	
}

void CTP_SendByte(u8 dat)
{
	u8 i;
	CTP_SDA_OUT(); 
	CTP_SCL_Clr();
	for(i=0;i<8;i++)
	{
	 CT_IIC_SDA((dat&0x80)>>7);
   dat<<=1; 
	 CTP_SCL_Set();
	 delay_us(5);
		CTP_SCL_Clr();
		delay_us(5);
	}
}
			 				     

u8 CTP_ReadByte(u8 ack)
{
	u8 i,dat;
	CTP_SDA_IN();
	for(i=0;i<8;i++)
	{
		CTP_SCL_Clr();
		delay_us(5);
		CTP_SCL_Set();
		dat<<=1;
		if(CTP_ReadSDA())
			dat++;
	}
		if(!ack)
		{
			CTP_IICNack();
		}
		else
		{
			CTP_IICAck();
		}
		return dat;
}




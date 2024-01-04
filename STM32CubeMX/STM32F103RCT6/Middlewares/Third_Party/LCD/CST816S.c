#include "CST816S.h"
#include "lcd.h"

const u16 POINT_COLOR_TBL[CTP_MAX_TOUCH]={RED,GREEN}; //触摸笔颜色
const u16 TPX[]={0x03,0x09}; //触摸地址
_m_tp_dev tp_dev=
{
 CST816S_Init,
 CST816S_Scan,
 0,
 0,	
 0,
};


//写数据
u8 CST816S_WR_DATA(u8 addr,u8 data)
{
	u8 reg;
	CTP_IIC_Start();          //IIC起始信号
	CTP_SendByte(CST_WR_CMD);  //发送写指令
	CTP_WaitAck();    
	CTP_SendByte(addr);
	CTP_WaitAck();
	CTP_SendByte(data);       //写入数据
	CTP_WaitAck();
	CTP_IIC_Stop();           //IIC结束信号
	reg=SUCCESS;
	return reg;
}

//读数据
u8 CST816S_RD_DATA(u8 addr,u8 len,u8 *value)
{
	u8 reg=ERROR,i;
	CTP_IIC_Start();
	CTP_SendByte(CST_WR_CMD);  //发送写指令
	CTP_WaitAck();    
	CTP_SendByte(addr);
	CTP_WaitAck();
  CTP_IIC_Start();
	CTP_SendByte(CST_RD_CMD);
	CTP_WaitAck();
	for(i=0;i<len;i++)
	{
		if(i==(len-1))
		{
			value[i]=CTP_ReadByte(0x00);
		}
		else
		{
			value[i]=CTP_ReadByte(0x01);
		}
	}
	CTP_IIC_Stop();
	reg=SUCCESS;
	return reg;
}

void  CST816S_Init(void)
{
	u8 ID;
	CTP_GPIOInit();
	CTP_RST_Clr();
  CTP_INT_Clr();
	HAL_Delay(50);
	CTP_INT_Set();
	HAL_Delay(50);
	CTP_RST_Set();
  HAL_Delay(50);
  CTP_INT_IN();	
	HAL_Delay(20);
	CST816S_RD_DATA(CST816S_ID,1,&ID);
}


//#include <stdio.h>
u8 CST816S_Scan(u8 mode)
{
	u8 res=0;
	u8 data[4] = {0};
	CST816S_RD_DATA(XposH,4,data);
	u16 x = (u16)((data[0]&0x0F)<<8)|data[1];
	u16 y = (u16)((data[2]&0x0F)<<8)|data[3];
	if (x < LCD_W && y < LCD_H) {
		if (tp_dev.sta & 0x01) {
			tp_dev.x[1] = tp_dev.x[0];
			tp_dev.y[1] = tp_dev.y[0];
			tp_dev.x[0] = x;
			tp_dev.y[0] = y;
			tp_dev.sta |= 0x02;
		} else {
			tp_dev.x[0] = x;
			tp_dev.y[0] = y;
			tp_dev.sta |= 0x01;
		}
//		printf("x: %d, y: %d\r\n", (int)tp_dev.x[0], (int)tp_dev.y[0]);
	} else {
		tp_dev.sta &= ~0x01;
		tp_dev.x[0] = 0;
		tp_dev.y[0] = 0;
	}
	return res;
}



void CST816S_test(void)
{
	u8 t=0; 	    
 	u16 lastpos[2][2];		//最后一次的数据 
	LCD_Fill(0,0,LCD_W-1,LCD_H-1,WHITE);
	while(1)
	{   
		tp_dev.scan(0);
		for(t=0;t<CTP_MAX_TOUCH;t++)
		{
			if((tp_dev.sta&(1<<t)))//判断是否有点触摸？
			{    
				HAL_Delay(1);
				if(tp_dev.x[t]<LCD_W&&tp_dev.y[t]<LCD_H)//在LCD范围内
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					LCD_DrawLine(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],POINT_COLOR_TBL[t]);
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
				}
			}else {lastpos[t][0]=0XFFFF;}
		}
		HAL_Delay(5);
	}	
}











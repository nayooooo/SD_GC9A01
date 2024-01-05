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


#include <stdio.h>
u8 CST816S_Scan(u8 mode)
{
	static u8 t = 0;
	u8 res=0;
	u8 data[4] = {0};
	u16 x, y;
	u8 finger_num, gesture_id;
	t++;
	if (t % 10) {
		return 1;
	} else {
		t = 0;
	}
	if ((tp_dev.sta & TP_DEV_STA_POS0_TOUCHED)
		&& (tp_dev.sta & TP_DEV_STA_POS1_TOUCHED)) {
		return 2;
	}
	
	CST816S_RD_DATA(FingerNum,1,&finger_num);
	CST816S_RD_DATA(GestureID,1,&gesture_id);
	if (gesture_id == 0x0C) tp_dev.sta |= TP_DEV_STA_POS0_LONGTOUCH;
	if (finger_num) {
		CST816S_RD_DATA(XposH,4,data);
		x = (u16)((data[0]&0x0F)<<8)|data[1];
		y = (u16)((data[2]&0x0F)<<8)|data[3];
	} else {
		if (!(tp_dev.sta & TP_DEV_STA_POS0_LONGTOUCH)) return 3;
		x = tp_dev.x[0];
		y = tp_dev.y[0];
	}
	if (x < LCD_W && y < LCD_H) {
		if (tp_dev.sta & TP_DEV_STA_POS0_TOUCHED) {
			tp_dev.x[1] = tp_dev.x[0];
			tp_dev.y[1] = tp_dev.y[0];
			tp_dev.sta |= TP_DEV_STA_POS1_TOUCHED;
		}
		switch (get_lcd_horizontal())
		{
			case 0:
				tp_dev.x[0] = LCD_W - x;
				tp_dev.y[0] = y;
				break;
			case 1:
				tp_dev.x[0] = x;
				tp_dev.y[0] = LCD_H - y;
				break;
			case 2:
				tp_dev.x[0] = x;
				tp_dev.y[0] = y;
				break;
			case 3:
				tp_dev.x[0] = LCD_W - x;
				tp_dev.y[0] = LCD_H - y;
				break;
			default:
				return 4;
		}
		if ((!(tp_dev.sta & TP_DEV_STA_POS0_LONGTOUCH)) || (gesture_id == 0x00)) {
			tp_dev.sta |= TP_DEV_STA_POS0_TOUCHED;
			printf("[sta: 0x%X] x: %d, y: %d\r\n", tp_dev.sta, (int)tp_dev.x[0], (int)tp_dev.y[0]);
		} else return 5;
	} else {
		tp_dev.sta &= ~0xFF;
		tp_dev.x[0] = 0;
		tp_dev.y[0] = 0;
	}
	return res;
}











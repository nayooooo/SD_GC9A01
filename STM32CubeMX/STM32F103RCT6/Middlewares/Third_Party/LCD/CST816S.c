#include "CST816S.h"
#include "lcd.h"

const u16 POINT_COLOR_TBL[CTP_MAX_TOUCH]={RED,GREEN}; //��������ɫ
const u16 TPX[]={0x03,0x09}; //������ַ
_m_tp_dev tp_dev=
{
 CST816S_Init,
 CST816S_Scan,
 0,
 0,	
 0,
};


//д����
u8 CST816S_WR_DATA(u8 addr,u8 data)
{
	u8 reg;
	CTP_IIC_Start();          //IIC��ʼ�ź�
	CTP_SendByte(CST_WR_CMD);  //����дָ��
	CTP_WaitAck();    
	CTP_SendByte(addr);
	CTP_WaitAck();
	CTP_SendByte(data);       //д������
	CTP_WaitAck();
	CTP_IIC_Stop();           //IIC�����ź�
	reg=SUCCESS;
	return reg;
}

//������
u8 CST816S_RD_DATA(u8 addr,u8 len,u8 *value)
{
	u8 reg=ERROR,i;
	CTP_IIC_Start();
	CTP_SendByte(CST_WR_CMD);  //����дָ��
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
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	u8 tempsta;
 	static u8 t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	t++;
	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
	{ 
		CST816S_RD_DATA(GT_ADDR_REG,1,&mode);
		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;	
			CST816S_WR_DATA(GT_ADDR_REG,temp);
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			tempsta=tp_dev.sta;			//���浱ǰ��tp_dev.staֵ
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[1]=tp_dev.x[0];	//���津��0������
			tp_dev.y[1]=tp_dev.y[0];
			for(i=0;i<2;i++)
			{
				if(tp_dev.sta&(1<<i))	//������Ч?
				{
					CST816S_RD_DATA(TPX[i],4,buf);
          if(USE_HORIZONTAL==2)//����
					{
						tp_dev.y[i]=buf[1];
						tp_dev.x[i]=buf[3];
					}
					else if(USE_HORIZONTAL==0)
					{
						tp_dev.x[i]=240-buf[1];
						tp_dev.y[i]=buf[3];
					} 
					else if(USE_HORIZONTAL==1)
					{
						tp_dev.x[i]=buf[1];
						tp_dev.y[i]=240-buf[3];
					} 
					else 
					{
						tp_dev.y[i]=240-buf[1];
						tp_dev.x[i]=240-buf[3];
					} 					
//					printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]>LCD_W||tp_dev.y[0]>LCD_H)//�Ƿ�����(���곬����)
			{ 
				if((mode&0XF)>1)		//��������������,�򸴵ڶ�����������ݵ���һ������.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//����һ��,��������������10��,�Ӷ����������
				}else					//�Ƿ�����,����Դ˴�����(��ԭԭ����)  
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					mode=0X80;		
					tp_dev.sta=tempsta;	//�ָ�tp_dev.sta
				}
			}else t=0;					//����һ��,��������������10��,�Ӷ����������
		}
	}
	else//�޴����㰴��
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);	//��ǰ����ɿ�
			tp_dev.x[0]=0;
			tp_dev.y[0]=0;
			tp_dev.sta&=0XE0;	//�������Ч���
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0;
			tp_dev.y[0]=0;
			tp_dev.sta&=0XE0;	//�������Ч���	
		}	 
	} 	
	if(t>240)t=10;//���´�10��ʼ����
	return res;
}



void CST816S_test(void)
{
	u8 t=0; 	    
 	u16 lastpos[2][2];		//���һ�ε����� 
	LCD_Fill(0,0,LCD_W-1,LCD_H-1,WHITE);
	while(1)
	{   
		tp_dev.scan(0);
		for(t=0;t<CTP_MAX_TOUCH;t++)
		{
			if((tp_dev.sta&(1<<t)))//�ж��Ƿ��е㴥����
			{    
				HAL_Delay(1);
				if(tp_dev.x[t]<LCD_W&&tp_dev.y[t]<LCD_H)//��LCD��Χ��
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











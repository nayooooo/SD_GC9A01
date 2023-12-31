#ifndef _CST816S_H_
#define _CST816S_H_

#include "main.h"
#include "CTIIC.h"

#define CST_WR_CMD 0x2A  //д�����ַ
#define CST_RD_CMD 0x2B  //д���ݵ�ַ

#define TP_PRES_DOWN 0x81  		//����������	  
#define TP_CATH_PRES 0x40  		//�а��������� 

#define CTP_MAX_TOUCH 2       

#define  FT_DEVIDE_MODE 0x00	   // CST816ģʽ���ƼĴ���
#define  GT_ADDR_REG    0x02     //����״̬�Ĵ���
#define  CST816S_ID     0xA7     //CST816SID�Ĵ���

typedef struct
{
	void (*init)(void);   //����IC��ʼ��
	u8 (*scan)(u8);       //����ɨ��
	u16 x[CTP_MAX_TOUCH]; //
	u16 y[CTP_MAX_TOUCH];
	u8  sta;              //����״̬  
}_m_tp_dev;


u8 CST816S_WR_DATA(u8 addr,u8 data);
u8 CST816S_RD_DATA(u8 addr,u8 len,u8 *value);
void  CST816S_Init(void);
u8  CST816S_Scan(u8 mode);
void CST816S_test(void);
#endif


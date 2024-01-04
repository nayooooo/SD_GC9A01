#ifndef _CST816S_H_
#define _CST816S_H_

#include "main.h"
#include "CTIIC.h"

#define CST_WR_CMD 0x2A  //д�����ַ
#define CST_RD_CMD 0x2B  //д���ݵ�ַ

#define TP_PRES_DOWN 0x81  		//����������	  
#define TP_CATH_PRES 0x40  		//�а��������� 

#define CTP_MAX_TOUCH 2       

//CST816�Ĵ���
#define GestureID			0x01		//���ƼĴ���
#define FingerNum			0x02		//��ָ����
#define XposH				0x03		//x����λ
#define XposL				0x04		//x�Ͱ�λ
#define YposH				0x05		//y����λ
#define YposL				0x06		//y�Ͱ�λ
#define ChipID				0xA7		//оƬ�ͺ�
#define	MotionMask		    0xEC		//��������
#define AutoSleepTime	    0xF9		//�Զ�����
#define IrqCrl				0xFA		//�жϿ���
#define AutoReset			0xFB		//����������
#define LongPressTime	    0xFC		//��������
#define DisAutoSleep	    0xFE		//ʹ�ܵ͹���ģʽ

#define CST816S_ID			ChipID     //CST816SID�Ĵ���
#define FT_DEVIDE_MODE		0x00	   // CST816ģʽ���ƼĴ���
#define GT_ADDR_REG			FingerNum     //����״̬�Ĵ���

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


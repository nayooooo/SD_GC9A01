#ifndef _CST816S_H_
#define _CST816S_H_

#include "main.h"
#include "CTIIC.h"

#define CST_WR_CMD 0x2A  //д�����ַ
#define CST_RD_CMD 0x2B  //д���ݵ�ַ

#define TP_PRES_DOWN 0x81  		//����������	  
#define TP_CATH_PRES 0x40  		//�а��������� 

#define CTP_MAX_TOUCH 1

//CST816�Ĵ���
#define GestureID			0x01		// ���ƼĴ���
#define FingerNum			0x02		// ��ָ����
#define XposH				0x03		// x����λ
#define XposL				0x04		// x�Ͱ�λ
#define YposH				0x05		// y����λ
#define YposL				0x06		// y�Ͱ�λ
#define ChipID				0xA7		// оƬ�ͺ�
#define	MotionMask		    0xEC		// ��������
#define AutoSleepTime	    0xF9		// �Զ�����
#define IrqCrl				0xFA		// �жϿ���
#define AutoReset			0xFB		// ����������
#define LongPressTime	    0xFC		// ��������
#define DisAutoSleep	    0xFE		// ʹ�ܵ͹���ģʽ

#define GID_NO_GESTURES		0x00		// ������
#define GID_SLIDE_UP		0x01		// �ϻ�
#define GID_SLIDE_DOWN		0x02		// �»�
#define GID_SLIDE_LEFT		0x03		// ��
#define GID_SLIDE_RIGHT		0x04		// �һ�
#define GID_CLICK			0x05		// ����
#define GID_DOUBLE_CLICK	0x0B		// ˫��
#define GID_LONGTOUCH		0x0C		// ����

#define CST816S_ID			ChipID     //CST816SID�Ĵ���
#define FT_DEVIDE_MODE		0x00	   // CST816ģʽ���ƼĴ���
#define GT_ADDR_REG			FingerNum     //����״̬�Ĵ���

typedef struct
{
	void (*init)(void);   //����IC��ʼ��
	u8 (*scan)(u8);       //����ɨ��
	u16 x[CTP_MAX_TOUCH]; //
	u16 y[CTP_MAX_TOUCH];
	u8  gid[CTP_MAX_TOUCH];
	u8  sta;              //����״̬  
}_m_tp_dev;

#define TP_DEV_STA_POS0_TOUCHED			(0x01)


u8 CST816S_WR_DATA(u8 addr,u8 data);
u8 CST816S_RD_DATA(u8 addr,u8 len,u8 *value);
void  CST816S_Init(void);
u8  CST816S_Scan(u8 mode);
#endif


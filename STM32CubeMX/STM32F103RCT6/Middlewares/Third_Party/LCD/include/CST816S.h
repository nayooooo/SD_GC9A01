#ifndef _CST816S_H_
#define _CST816S_H_

#include "main.h"
#include "CTIIC.h"

#define CST_WR_CMD 0x2A  //写命令地址
#define CST_RD_CMD 0x2B  //写数据地址

#define TP_PRES_DOWN 0x81  		//触屏被按下	  
#define TP_CATH_PRES 0x40  		//有按键按下了 

#define CTP_MAX_TOUCH 1

//CST816寄存器
#define GestureID			0x01		// 手势寄存器
#define FingerNum			0x02		// 手指数量
#define XposH				0x03		// x高四位
#define XposL				0x04		// x低八位
#define YposH				0x05		// y高四位
#define YposL				0x06		// y低八位
#define ChipID				0xA7		// 芯片型号
#define	MotionMask		    0xEC		// 触发动作
#define AutoSleepTime	    0xF9		// 自动休眠
#define IrqCrl				0xFA		// 中断控制
#define AutoReset			0xFB		// 无手势休眠
#define LongPressTime	    0xFC		// 长按休眠
#define DisAutoSleep	    0xFE		// 使能低功耗模式

#define GID_NO_GESTURES		0x00		// 无手势
#define GID_SLIDE_UP		0x01		// 上滑
#define GID_SLIDE_DOWN		0x02		// 下滑
#define GID_SLIDE_LEFT		0x03		// 左滑
#define GID_SLIDE_RIGHT		0x04		// 右滑
#define GID_CLICK			0x05		// 单击
#define GID_DOUBLE_CLICK	0x0B		// 双击
#define GID_LONGTOUCH		0x0C		// 长按

#define CST816S_ID			ChipID     //CST816SID寄存器
#define FT_DEVIDE_MODE		0x00	   // CST816模式控制寄存器
#define GT_ADDR_REG			FingerNum     //触摸状态寄存器

typedef struct
{
	void (*init)(void);   //触摸IC初始化
	u8 (*scan)(u8);       //触摸扫描
	u16 x[CTP_MAX_TOUCH]; //
	u16 y[CTP_MAX_TOUCH];
	u8  gid[CTP_MAX_TOUCH];
	u8  sta;              //触摸状态  
}_m_tp_dev;

#define TP_DEV_STA_POS0_TOUCHED			(0x01)


u8 CST816S_WR_DATA(u8 addr,u8 data);
u8 CST816S_RD_DATA(u8 addr,u8 len,u8 *value);
void  CST816S_Init(void);
u8  CST816S_Scan(u8 mode);
#endif


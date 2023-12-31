#ifndef _CTIIC_H_
#define _CTIIC_H_

#include "main.h"


#define	CTP_RST_Set()	 CTP_RST_GPIO_Port->BSRR=CTP_RST_Pin
#define	CTP_INT_Set()    CTP_INT_GPIO_Port->BSRR=CTP_INT_Pin
#define CTP_SCL_Set()	 CTP_SCL_GPIO_Port->BSRR=CTP_SCL_Pin
#define CTP_SDA_Set()	 CTP_SDA_GPIO_Port->BSRR=CTP_SDA_Pin

#define	CTP_RST_Clr()	 CTP_RST_GPIO_Port->BRR=CTP_RST_Pin
#define	CTP_INT_Clr()    CTP_INT_GPIO_Port->BRR=CTP_INT_Pin
#define CTP_SCL_Clr()	 CTP_SCL_GPIO_Port->BRR=CTP_SCL_Pin
#define CTP_SDA_Clr()	 CTP_SDA_GPIO_Port->BRR=CTP_SDA_Pin

#define CTP_ReadSDA()	 HAL_GPIO_ReadPin(CTP_SDA_GPIO_Port, CTP_SDA_Pin)
#define CT_IIC_SDA(x)	 HAL_GPIO_WritePin(CTP_SDA_GPIO_Port, CTP_SDA_Pin, x)


void CTP_SDA_IN(void);
void CTP_SDA_OUT(void);
void CTP_GPIOInit(void);
void CTP_IIC_Start(void);
void CTP_IIC_Stop(void);
u8 CTP_WaitAck(void);
void CTP_IICAck(void);
void CTP_IICNack(void);
void CTP_SendByte(u8 dat);
u8 CTP_ReadByte(u8 ack);
void CTP_INT_IN(void); 


#endif

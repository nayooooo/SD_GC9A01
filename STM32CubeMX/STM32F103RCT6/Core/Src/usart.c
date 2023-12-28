/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

#if 1

#include <stdio.h>

FILE __stdout;

void __sys_exit(int x)
{
	x = x;
}

int fputc(int ch, FILE *f)
{
	while ((USART1->SR & UART_FLAG_TC) == 0);
	USART1->DR = (uint8_t)ch;
	return ch;
}

#endif

uint8_t UART1_aRxBuffer;

/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  
  HAL_UART_Receive_IT(&huart1, &UART1_aRxBuffer, 1);

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

// UART1
#define UART1_RX_BUFF_SIZE		(200)
static uint8_t UART1_Rx_Buff[UART1_RX_BUFF_SIZE] = { 0 };
static int UART1_Rx_Front = 0;
static int UART1_Rx_Rear = 0;

// 得到接收缓冲区未读取的数据长度
int Get_UART1_Rx_Len(void)
{
	return (UART1_Rx_Rear - UART1_Rx_Front + UART1_RX_BUFF_SIZE) % UART1_RX_BUFF_SIZE;
}

// 往缓冲区写入一个字符
int Write_UART1_Rx(uint8_t ch)
{
	// 缓冲区满
	if (Get_UART1_Rx_Len() >= UART1_RX_BUFF_SIZE - 1) return -1;
	UART1_Rx_Buff[UART1_Rx_Rear] = ch;
	UART1_Rx_Rear = (UART1_Rx_Rear + 1) % UART1_RX_BUFF_SIZE;
	return 0;
}

int Read_UART1_Rx(void)
{
	if (Get_UART1_Rx_Len() <= 0) return -1;
	int ch = UART1_Rx_Buff[UART1_Rx_Front];
	if (ch < 0) return -2;
	UART1_Rx_Front = (UART1_Rx_Front + 1) % UART1_RX_BUFF_SIZE;
	return (int)ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1) {
		// 缓冲区满
		if (Get_UART1_Rx_Len() >= UART1_RX_BUFF_SIZE - 1) goto UART1_err_out;
		if (Write_UART1_Rx(UART1_aRxBuffer)) goto UART1_err_out;
	UART1_err_out:
		HAL_UART_Receive_IT(huart, &UART1_aRxBuffer, 1);
		
	}
}

/* USER CODE END 1 */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "fatfs.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include "malloc.h"

#include "fft.h"

#include "at_user.h"

#include "lcd.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

FATFS fs;
FATFS* pfs = &fs;
FIL f;
FRESULT fres;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

//void HAL_Delay(__IO uint32_t Delay)
//{
//	while (Delay) {
//		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
//			Delay--;
//		}
//	}
//}

void FatFS_Test_SD_Card(void)
{
  fres = f_mount(&fs, "sd", 1);
  printf("f_mount result: 0x%02X\r\n", fres);
  if (fres != FR_OK) {
	  printf("f_mount failed!\r\n");
	  return;
  }
  DWORD fre_clust;
  if (f_getfree("sd", &fre_clust, &pfs) != FR_OK) {
	  printf("f_getfree failed!\r\n");
	  goto error_out;
  }
  uint32_t totalSpace, freeSpace;  // unit(KB)
  totalSpace = (uint32_t)(((pfs->n_fatent - 2) * pfs->csize) >> 1);
  freeSpace = (uint32_t)((fre_clust * pfs->csize) >> 1);
  printf("totalSpace=%uKB, totalBlock=0x%X\r\n", totalSpace, totalSpace << 1);
  printf("freeSpace=%uKB\r\n", freeSpace);
  printf("total: %.2fGB, free: %.2fGB\r\n", (float)totalSpace / 1024 / 1024, (float)freeSpace / 1024 / 1024);
error_out:
  fres = f_mount(&fs, "sd", 0);
  printf("f_unmount result: 0x%02X\r\n", fres);
  if (fres != FR_OK) {
	  printf("f_unmount failed!\r\n");
	  return;
  }
}

void LCD_Test_Fps(void)
{
	uint32_t lcd_frames = 0;
	uint32_t lcd_test_fps_start_time;
	uint32_t lcd_test_time = 0;
	
	lcd_test_fps_start_time = HAL_GetTick();
	
	while (lcd_test_time <= 10000) {
		LCD_Fill(0, 0, LCD_W, LCD_H, lcd_frames++);
		lcd_test_time = HAL_GetTick() - lcd_test_fps_start_time;
	}
	float lcd_fps = (float)lcd_frames / ((float)lcd_test_time / 1000);
	LCD_ShowString(20, 104, (const u8*)"lcd test ok!", RED, WHITE, 16, 0);
	LCD_ShowString(20, 120, (const u8*)"test time: 10s", RED, WHITE, 16, 0);
	LCD_ShowString(20, 136, (const u8*)"total frame: ", RED, WHITE, 16, 0);
	LCD_ShowIntNum(134, 136, lcd_frames, 5, RED, WHITE, 16);
	LCD_ShowString(20, 152, (const u8*)"lcd fps: ", RED, WHITE, 16, 0);
	LCD_ShowFloatNum1(92, 152, lcd_fps, 5, RED, WHITE, 16);
}

void LCD_Draw_Star(void)
{
  fres = f_mount(&fs, "sd", 1);
  printf("f_mount result: 0x%02X\r\n", fres);
  if (fres != FR_OK) {
	  printf("f_mount failed!\r\n");
	  return;
  }
  
  fres = f_open(&f, "/data/FFT/star.txt", FA_READ);
  if (fres != FR_OK) {
	  printf("f_mount failed!\r\n");
	  goto error_out;
  }
  char buffer[30];
  complex star[8];
  complex star_fft[8];
  for (int i = 0; i < 8; i++) {
	  f_gets(buffer, sizeof(buffer), &f);
	  sscanf(buffer, "%lf %lf\n", &(star[i].real), &(star[i].imag));
	  printf("[%d] %+.4f, %+.4f\r\n", i, star[i].real, star[i].imag);
  }
  fft(star, 8, star_fft, 8);
  for (int i = 0; i < 8; i++) {
	  star_fft[i].imag = 240.0 - star_fft[i].imag;
  }
  for (int i = 0; i < 8; i++) {
	  printf("[FFT %d] (%u, %u)\r\n", i, (u16)(star_fft[i].real), (u16)(star_fft[i].imag));
	  LCD_DrawLine((u16)(star_fft[i].real), (u16)(star_fft[i].imag),
					(u16)(star_fft[(i+1)%8].real), (u16)(star_fft[(i+1)%8].imag), RED);
  }
  f_close(&f);
  
error_out:
  fres = f_mount(&fs, "sd", 0);
  printf("f_unmount result: 0x%02X\r\n", fres);
  if (fres != FR_OK) {
	  printf("f_unmount failed!\r\n");
	  return;
  }
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  
  printf("\r\n");
  
  my_mem_init(SRAMIN);
  
  if (at_user_init() == AT_EOK) {
	  printf("at user initialize ok!\r\n");
  } else {
	  printf("at user initialize failed!\r\n");
  }
  
  LCD_Init();
  LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
  
  FatFS_Test_SD_Card();
  
  printf("STM32F103C8T6 initialize OK!\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  LCD_Draw_Star();
  
  while (1)
  {
	  at.handleAuto(&at);
	  
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

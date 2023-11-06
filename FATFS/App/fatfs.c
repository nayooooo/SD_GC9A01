/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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
  
#include "rtc.h"
  
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
	extern RTC_HandleTypeDef hrtc;
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	DWORD timeStamp = 0x00000000;
	timeStamp |= (uint8_t)((int)date.Year - RTC_USER_DATE_YEAR_DELTA - 1980); timeStamp <<= 4;
	timeStamp |= (uint8_t)date.Month; timeStamp <<= 5;
	timeStamp |= (uint8_t)date.Date; timeStamp <<= 5;
	timeStamp |= (uint8_t)time.Hours; timeStamp <<= 6;
	timeStamp |= (uint8_t)time.Minutes; timeStamp <<= 5;
	timeStamp |= (uint8_t)time.Seconds >> 1;
	return timeStamp;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

// 如果编译报错，可能是CubeMX重新生成的时候，重置了syscall.c文件，将其中的相关函数注释即可
#include "malloc.h"

void* ff_memalloc (	/* Returns pointer to the allocated memory block */
	UINT msize		/* Number of bytes to allocate */
)
{
	return mymalloc(0, msize);	/* Allocate a new memory block with POSIX API */
}

void ff_memfree (
	void* mblock	/* Pointer to the memory block to free */
)
{
	myfree(0, mblock);	/* Discard the memory block with POSIX API */
}

/* USER CODE END Application */

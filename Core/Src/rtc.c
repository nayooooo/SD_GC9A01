/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */

#define RTC_USER_TIME_SET_BKP_BR		RTC_BKP_DR10
#define RTC_USER_TIME_SET_BKP_BR_VAL	(0x5F5A)

#define RTC_USER_TIME_HOUR_BKP_BR		RTC_BKP_DR7
#define RTC_USER_TIME_MINUTE_BKP_BR		RTC_BKP_DR6
#define RTC_USER_TIME_SECOND_BKP_BR		RTC_BKP_DR5

#define RTC_USER_DATE_YEAR_BKP_BR		RTC_BKP_DR4
#define RTC_USER_DATE_MONTH_BKP_BR		RTC_BKP_DR3
#define RTC_USER_DATE_DATE_BKP_BR		RTC_BKP_DR2
#define RTC_USER_DATE_WDATE_BKP_BR		RTC_BKP_DR1

#define YEAR ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
    + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))
// January February March April May June July August September October November December
#define MONTH (__DATE__ [0] == 'J' ? (__DATE__ [1] == 'a' ? 1 : __DATE__ [2] == 'n' ? 6 : 7) \
	: __DATE__ [0] == 'F' ? 2 \
	: __DATE__ [0] == 'M' ? (__DATE__ [2] == 'r' ? 3 : 5) \
	: __DATE__ [0] == 'A' ? (__DATE__ [1] == 'p' ? 4 : 8) \
	: __DATE__ [0] == 'S' ? 9 \
	: __DATE__ [0] == 'O' ? 10 \
	: __DATE__ [0] == 'N' ? 11 : 12)
#define DAY ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
    + (__DATE__ [5] - '0'))
#define DATE_AS_INT (((YEAR - 2000) * 12 + MONTH) * 31 + DAY)

static uint8_t rtc_user_get_wdate(RTC_DateTypeDef* sDate)
{
	if (sDate == NULL) return -1;
	
	int year, y, m, d, c;
	year = (int)sDate->Year - RTC_USER_DATE_YEAR_DELTA;
	m = (int)sDate->Month;
	d = (int)sDate->Date;
	if (m == 1 || m == 2) {
		year--;
		m += 12;
	}
	c = year / 100;
	y = year - c * 100;
	int week, wDay;
	week = y + y / 4 + c / 4 - 2 * c + 26 * (m + 1) / 10 + d - 1;
	while (week < 0) week += 7;
	wDay = week % 7;
	sDate->WeekDay = (uint8_t)wDay;
	
	return sDate->WeekDay;
}

#define HOUR ((__TIME__ [0] == ' ' ? 0 : __TIME__ [0] - '0') * 10 \
	+ __TIME__ [1] - '0')
#define MINUTE ((__TIME__ [3] == ' ' ? 0 : __TIME__ [3] - '0') * 10 \
	+ __TIME__ [4] - '0')
#define SECOND ((__TIME__ [6] == ' ' ? 0 : __TIME__ [6] - '0') * 10 \
	+ __TIME__ [7] - '0')

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
  if (1) {
	  const char* weekDay[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	  RTC_TimeTypeDef time;
	  RTC_DateTypeDef date;
	  if (HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN) != HAL_OK) {
		  Error_Handler();
	  }
	  rtc_user_date_read_bkp(&date);
	  #include <stdio.h>
	  printf("Last power outage on: \r\n");
	  printf("\t%04d-%02d-%02d %s %02d:%02d:%02d\r\n",
				(int)date.Year - RTC_USER_DATE_YEAR_DELTA, date.Month, date.Date, weekDay[date.WeekDay],
				time.Hours, time.Minutes, time.Seconds);
  }
  
  if (HAL_RTCEx_BKUPRead(&hrtc, RTC_USER_TIME_SET_BKP_BR) != RTC_USER_TIME_SET_BKP_BR_VAL) {
	  HAL_RTCEx_BKUPWrite(&hrtc, RTC_USER_TIME_SET_BKP_BR, RTC_USER_TIME_SET_BKP_BR_VAL);
	  RTC_TimeTypeDef time;
	  time.Hours = HOUR;
	  time.Minutes = MINUTE;
	  time.Seconds = SECOND;
	  if (HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN) != HAL_OK) {
		  Error_Handler();
	  }
	  rtc_user_time_write_bkp(&time);
	  RTC_DateTypeDef date;
	  date.Year = (uint8_t)(YEAR + RTC_USER_DATE_YEAR_DELTA);
	  date.Month = MONTH;
	  date.Date = DAY;
	  date.WeekDay = rtc_user_get_wdate(&date);
	  if (HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK) {
		  Error_Handler();
	  }
	  rtc_user_date_write_bkp(&date);
  }
  if (1) {
	  RTC_DateTypeDef date;
	  rtc_user_date_read_bkp(&date);
	  if (HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK) {
		  Error_Handler();
	  }
	  rtc_user_calibration_date();  // 先想办法将时间保存下来
	  if (HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK) {
		  Error_Handler();
	  }
	  rtc_user_date_write_bkp(&date);
  }

  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */
	  
	  __HAL_RTC_ALARM_ENABLE_IT(rtcHandle, RTC_AUTO_1_SECOND);

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */
	  
	  __HAL_RTC_ALARM_DISABLE_IT(rtcHandle, RTC_AUTO_1_SECOND);

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

HAL_StatusTypeDef rtc_user_time_write_bkp(RTC_TimeTypeDef* time)
{
	if (time == NULL) return HAL_ERROR;
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_USER_TIME_HOUR_BKP_BR, time->Hours);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_USER_TIME_MINUTE_BKP_BR, time->Minutes);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_USER_TIME_SECOND_BKP_BR, time->Seconds);
	return HAL_OK;
}

HAL_StatusTypeDef rtc_user_time_read_bkp(RTC_TimeTypeDef* time)
{
	if (time == NULL) return HAL_ERROR;
	time->Hours = HAL_RTCEx_BKUPRead(&hrtc, RTC_USER_TIME_HOUR_BKP_BR);
	time->Minutes = HAL_RTCEx_BKUPRead(&hrtc, RTC_USER_TIME_MINUTE_BKP_BR);
	time->Seconds = HAL_RTCEx_BKUPRead(&hrtc, RTC_USER_TIME_SECOND_BKP_BR);
	return HAL_OK;
}

HAL_StatusTypeDef rtc_user_date_write_bkp(RTC_DateTypeDef* date)
{
	if (date == NULL) return HAL_ERROR;
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_USER_DATE_YEAR_BKP_BR, date->Year);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_USER_DATE_MONTH_BKP_BR, date->Month);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_USER_DATE_DATE_BKP_BR, date->Date);
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_USER_DATE_WDATE_BKP_BR, date->WeekDay);
	return HAL_OK;
}

HAL_StatusTypeDef rtc_user_date_read_bkp(RTC_DateTypeDef* date)
{
	if (date == NULL) return HAL_ERROR;
	date->Year = HAL_RTCEx_BKUPRead(&hrtc, RTC_USER_DATE_YEAR_BKP_BR);
	date->Month = HAL_RTCEx_BKUPRead(&hrtc, RTC_USER_DATE_MONTH_BKP_BR);
	date->Date = HAL_RTCEx_BKUPRead(&hrtc, RTC_USER_DATE_DATE_BKP_BR);
	date->WeekDay = HAL_RTCEx_BKUPRead(&hrtc, RTC_USER_DATE_WDATE_BKP_BR);
	return HAL_OK;
}

// 校准日期，请确保掉电时长小于24小时
HAL_StatusTypeDef rtc_user_calibration_date(void)
{
	RTC_TimeTypeDef oldTime, nowTime;
	
	rtc_user_time_read_bkp(&oldTime);
	if (HAL_RTC_GetTime(&hrtc, &nowTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	long long oldSecs, nowSecs;
	oldSecs = (long long)oldTime.Seconds + 60 * (long long)oldTime.Minutes + 3600 * (long long)oldTime.Hours;
	nowSecs = (long long)nowTime.Seconds + 60 * (long long)nowTime.Minutes + 3600 * (long long)nowTime.Hours;
	if (oldSecs > nowSecs) {
		RTC_DateTypeDef date;
		if (HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK) {
			Error_Handler();
		}
		date.Date++;
		if ((date.Date > 31) &&
			(((date.Month <= 7) && (date.Month % 2)) ||
			 ((date.Month >= 8) && (!(date.Month % 2))))
		) {  // 31 days
			date.Date = 1;
			if (date.Month != 12) { date.Month++; }
			else { date.Month = 1; date.Year++; }
		} else if ((date.Date > 30) &&
			(((date.Month != 2) && (date.Month <= 7) && (!(date.Month % 2))) ||
			 (date.Month >= 8) && (date.Month % 2))
		) {  // 30 days
			date.Date = 1;
			date.Month++;
		} else if ((date.Date > 28) &&
			(date.Month == 2)
		) {  // 28 days
			int year = (int)date.Year - RTC_USER_DATE_YEAR_DELTA;
			if ((year % 400) && ((!(year % 4)) && (year % 100))) {
				date.Date = 1;
				date.Month++;
			}
		} else if ((date.Date > 29) &&
			(date.Month == 2)
		) {  // 29 days
			date.Date = 1;
			date.Month++;
		}
		if (HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN) != HAL_OK) {
			Error_Handler();
		}
	}
	
	return HAL_OK;
}

/* USER CODE END 1 */

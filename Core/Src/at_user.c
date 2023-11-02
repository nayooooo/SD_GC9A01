#include "at_user.h"

#include "usart.h"
#include "rtc.h"
#include "string.h"
static Stream at_user_stream_device;
static Stream* sdev = &at_user_stream_device;

int _at_user_sdev_print(Stream* this, const char* message)
{
	if (this == nullptr) return -1;
	int len = strlen(message);
	HAL_StatusTypeDef ret = HAL_UART_Transmit(&huart1, (uint8_t*)message, len, 0xFFFF);
	if (ret != HAL_OK) return -2;
	return len;
}

int _at_user_sdev_available(Stream* this)
{
	if (this == nullptr) return -1;
	
	return Get_UART1_Rx_Len();
}

int _at_user_sdev_read(Stream* this)
{
	if (this == nullptr) return -1;
	return Read_UART1_Rx();
}

static At_Err_t _at_user_AT(At_Param_t param);
static At_Err_t _at_user_AT_List(At_Param_t param);
static At_Err_t _at_user_AT_Reboot(At_Param_t param);
static At_Err_t _at_user_AT_Get_Time(At_Param_t param);
static At_Err_t _at_user_AT_LED(At_Param_t param);
static struct At_State _atTable[] = {
	{ "AT", AT_TYPE_CMD, _at_user_AT },
	{ "AT+LS", AT_TYPE_CMD, _at_user_AT_List },
	{ "AT+REBOOT", AT_TYPE_CMD, _at_user_AT_Reboot },
	{ "AT+TIME", AT_TYPE_CMD, _at_user_AT_Get_Time },
	{ "AT+LED", AT_TYPE_CMD, _at_user_AT_LED },
	{ AT_LABLE_TAIL, AT_TYPE_NULL, nullptr },
};
At at;
#define AT_USER_READSTRING_LEN		(100)
static char at_user_readString[AT_USER_READSTRING_LEN] = { 0 };

static At_Err_t _at_user_AT(At_Param_t param)
{
	at.sendInfor(&at, AT_USER_OK);
	return AT_EOK;
}

static At_Err_t _at_user_AT_List(At_Param_t param)
{
	at.printSet(&at, "at");
	return AT_EOK;
}

static At_Err_t _at_user_AT_Reboot(At_Param_t param)
{
	HAL_NVIC_SystemReset();
	return AT_EOK;
}

static At_Err_t _at_user_AT_LED_on(At_Param_t param);
static At_Err_t _at_user_AT_LED_off(At_Param_t param);
static struct At_State _at_user_LED_Table[] = {
	{ "on", AT_TYPE_CMD, _at_user_AT_LED_on },
	{ "off", AT_TYPE_CMD, _at_user_AT_LED_off },
	{ AT_LABLE_TAIL, AT_TYPE_NULL, nullptr },
};
static At _at_LED;

static At_Err_t _at_user_AT_LED_on(At_Param_t param)
{
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
	return AT_EOK;
}

static At_Err_t _at_user_AT_LED_off(At_Param_t param)
{
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
	return AT_EOK;
}

static At_Err_t _at_user_AT_Get_Time(At_Param_t param)
{
	const char* weekDay[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	at.printf(&at, "%04d-%02d-%02d %s %02d:%02d:%02d\r\n",
		(int)date.Year - RTC_USER_DATE_YEAR_DELTA, date.Month, date.Date, weekDay[date.WeekDay],
		time.Hours, time.Minutes, time.Seconds);
	return AT_EOK;
}

static At_Err_t _at_user_AT_LED(At_Param_t param)
{
	if (param->argc != 1) {
		_at_LED.printSet(&_at_LED, "at LED");
		at.sendInfor(&at, AT_USER_ERROR_ARGC);
		return AT_ERROR;
	}
	
	size_t len = strlen(param->argv[0]);
	char *str = (char*)at_malloc(len + 1);
	if (str == nullptr) {
		at.sendInfor(&at, AT_USER_ERROR_MALLOC);
		return AT_ERROR;
	}
	at_memset(str, 0, len + 1);
	at_memcpy(str, param->argv[0], len);
	At_Err_t err = _at_LED.handle(&_at_LED, str);
	at_free(str);
	
	if (err == AT_EOK) {
		at.sendInfor(&at, AT_USER_OK);
	} else if (err == AT_ERROR_NOT_FIND) {
		_at_LED.printSet(&_at_LED, "at LED");
		at.sendInfor(&at, AT_USER_ERROR_NOTFIND);
	} else {
		at.sendInfor(&at, AT_USER_ERROR_ARGV);
	}
	return err;
}

At_Err_t at_user_init(void)
{
	At_Err_t err = Stream_Init(sdev, 3, _at_user_sdev_print, _at_user_sdev_available, _at_user_sdev_read);
	if (err != AT_EOK) return err;
	err = At_Init(&at, _atTable, sdev, sdev, at_user_readString, AT_USER_READSTRING_LEN, 0);
	if (err != AT_EOK) return err;
	err = At_Create(&_at_LED, _at_user_LED_Table, sdev, sdev, 1, 10);
	return err;
}

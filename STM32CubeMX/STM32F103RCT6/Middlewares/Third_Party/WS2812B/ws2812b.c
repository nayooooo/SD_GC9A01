#include "ws2812b.h"

#include <string.h>

#define WS2812B_DI_SET_HAL()         WS_IN_GPIO_Port->BSRR=WS_IN_Pin
#define WS2812B_DI_RESET_HAL()       WS_IN_GPIO_Port->BRR=WS_IN_Pin
#define WS2812B_DI_SET()             WS2812B_DI_SET_HAL()
#define WS2812B_DI_RESET()           WS2812B_DI_RESET_HAL()

struct ws2812b_rgb ws_rgb[WS2812B_NODE_NUM];

static void ws2812b_delay_hh(void)
{
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
}

#define ws2812b_delay_hl()

static void ws2812b_delay_lh(void)
{
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
}

static void ws2812b_delay_ll(void)
{
    __NOP();__NOP();__NOP();
}

static void ws2812b_bit_1(void)
{
    WS2812B_DI_SET();
    ws2812b_delay_hh();
    WS2812B_DI_RESET();
    ws2812b_delay_hl();
}

static void ws2812b_bit_0(void)
{
    WS2812B_DI_SET();
    ws2812b_delay_ll();
    WS2812B_DI_RESET();
    ws2812b_delay_lh();
}

static void ws2812b_bit_reset(void)
{
    WS2812B_DI_RESET();
	delay_us(300);
}

static ws_err_t ws2812b_send_byte(uint8_t data)
{
    for (int i = 0; i < 8; i++) {
        if (data & 0X80) {
            ws2812b_bit_1();
        } else {
            ws2812b_bit_0();
        }
        data <<= 1;
    }
    return WS_EOK;
}

ws_err_t ws2812b_clear_rgb_buff(void)
{
    memset(ws_rgb, 0X00, sizeof(ws_rgb));

    return WS_EOK;
}

ws_err_t ws2812b_send_node(ws2812b_rgb_t rgb)
{
    ws2812b_send_byte(rgb->g);
    ws2812b_send_byte(rgb->r);
    ws2812b_send_byte(rgb->b);
    ws2812b_bit_reset();
    return WS_EOK;
}

ws_err_t ws2812b_send_nnode(ws2812b_rgb_t rgb, uint8_t num)
{
    for (uint8_t i = 0; i < num; i++) {
        ws2812b_send_byte(rgb[i].g);
        ws2812b_send_byte(rgb[i].r);
        ws2812b_send_byte(rgb[i].b);
    }
    ws2812b_bit_reset();
    return WS_EOK;
}

int ws2812b_init(void)
{
    return WS_EOK;
}

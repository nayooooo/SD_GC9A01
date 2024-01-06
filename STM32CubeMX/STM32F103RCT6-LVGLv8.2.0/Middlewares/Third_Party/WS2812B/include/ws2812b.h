#ifndef APPLICATIONS_WS2812B_H_
#define APPLICATIONS_WS2812B_H_

#include "main.h"

typedef uint32_t ws_err_t;
#define WS_EOK			(0)

struct ws2812b_rgb{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
typedef struct ws2812b_rgb * ws2812b_rgb_t;

#define WS2812B_NODE_NUM            (1)
extern struct ws2812b_rgb ws_rgb[WS2812B_NODE_NUM];

int ws2812b_init(void);
ws_err_t ws2812b_clear_rgb_buff(void);
ws_err_t ws2812b_send_node(ws2812b_rgb_t rgb);
ws_err_t ws2812b_send_nnode(ws2812b_rgb_t rgb, uint8_t num);

#endif /* APPLICATIONS_WS2812B_H_ */

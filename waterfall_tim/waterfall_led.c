#include "waterfall_led.h"
#include "stm32f4xx.h"

#define ROW_COUNT 4
#define COL_COUNT 4

#define ROW_GPIO_PORT GPIOA
#define COL_GPIO_PORT GPIOB

#define ROW_GPIO_PIN_0 GPIO_Pin_8
#define ROW_GPIO_PIN_1 GPIO_Pin_9
#define ROW_GPIO_PIN_2 GPIO_Pin_10
#define ROW_GPIO_PIN_3 GPIO_Pin_11
#define COL_GPIO_PIN_0 GPIO_Pin_12
#define COL_GPIO_PIN_1 GPIO_Pin_13
#define COL_GPIO_PIN_2 GPIO_Pin_14
#define COL_GPIO_PIN_3 GPIO_Pin_15

static uint8_t row = 0;
static uint8_t col = 0;

void LED_Waterfall_Init(void) {
    // 配置LED点阵的行和列GPIO为输出模式

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_InitStruct.GPIO_Pin = ROW_GPIO_PIN_0 | ROW_GPIO_PIN_1 | ROW_GPIO_PIN_2 | ROW_GPIO_PIN_3;
    GPIO_Init(ROW_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = COL_GPIO_PIN_0 | COL_GPIO_PIN_1 | COL_GPIO_PIN_2 | COL_GPIO_PIN_3;
    GPIO_Init(COL_GPIO_PORT, &GPIO_InitStruct);
}

void LED_Waterfall_Update(void) {
    // 关闭所有LED
    GPIO_Write(ROW_GPIO_PORT, 0x0000);
    GPIO_Write(COL_GPIO_PORT, 0xFFFF);

    // 点亮当前LED
    GPIO_WriteBit(ROW_GPIO_PORT, ROW_GPIO_PIN_0 << row, Bit_SET);
    GPIO_WriteBit(COL_GPIO_PORT, COL_GPIO_PIN_0 << col, Bit_RESET);

    // 更新下一个LED的位置
    col++;
    if (col >= COL_COUNT) {
        col = 0;
        row++;
        if (row >= ROW_COUNT) {
            row = 0;
        }
    }
}


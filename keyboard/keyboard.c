#include "stm32f10x.h"
#include "delay.h"

#define ROWS 4
#define COLS 4

// 定义行和列的引脚
GPIO_TypeDef* ROW_PORT[ROWS] = {GPIOA, GPIOA, GPIOA, GPIOA};
const uint16_t ROW_PIN[ROWS] = {GPIO_Pin_7, GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_4};
GPIO_TypeDef* COL_PORT[COLS] = {GPIOA, GPIOA, GPIOA, GPIOA};
const uint16_t COL_PIN[COLS] = {GPIO_Pin_3,GPIO_Pin_2, GPIO_Pin_1, GPIO_Pin_0};

// 矩阵键盘映射表
char keymap[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能引脚时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置行引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = ROW_PIN[0] | ROW_PIN[1] | ROW_PIN[2] | ROW_PIN[3];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置列引脚为上拉输入
    GPIO_InitStructure.GPIO_Pin = COL_PIN[0] | COL_PIN[1] | COL_PIN[2] | COL_PIN[3];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

char read_key(void)
{
    uint8_t row, col;

    // 扫描行
    for (row = 0; row < ROWS; row++) {
        GPIO_ResetBits(ROW_PORT[row], ROW_PIN[row]);

        // 延时一段时间，以确保行引脚稳定为低电平
        delay_ms(1);

        // 检测列
        for (col = 0; col < COLS; col++) {
            if (GPIO_ReadInputDataBit(COL_PORT[col], COL_PIN[col]) == Bit_RESET) {
                // 按键按下
                // 延时一段时间，以避免按键抖动
                delay_ms(10);

                // 再次检测确认按键是否仍然按下
                if (GPIO_ReadInputDataBit(COL_PORT[col], COL_PIN[col]) == Bit_RESET) {
                    // 按键仍然按下
                    // 停止扫描
                    GPIO_SetBits(ROW_PORT[row], ROW_PIN[row]);
                    return keymap[row][col];
                }
            }
        }

        GPIO_SetBits(ROW_PORT[row], ROW_PIN[row]);
    }

    // 没有按键按下
    return '\0';
}

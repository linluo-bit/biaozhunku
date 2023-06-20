#include "keyboard.h"
#include "delay.h"
#include "stm32f10x_tim.h"

GPIO_TypeDef* ROW_PORT[ROWS] = {GPIOA, GPIOA, GPIOA, GPIOA};
const uint16_t ROW_PIN[ROWS] = {GPIO_Pin_7, GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_4};
GPIO_TypeDef* COL_PORT[COLS] = {GPIOA, GPIOA, GPIOA, GPIOA};
const uint16_t COL_PIN[COLS] = {GPIO_Pin_3, GPIO_Pin_2, GPIO_Pin_1, GPIO_Pin_0};

char keymap[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void GPIO_Config(void)
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

void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_BaseStruct.TIM_Prescaler = 7200 - 1;  // 设置预分频器值，适配系统时钟频率
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 100 - 1;  // 设置定时器溢出时间，控制扫描的频率

    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIM2, &TIM_BaseStruct);

    TIM_Cmd(TIM2, ENABLE);
}

void Keyboard_Init(void)
{
    GPIO_Config();
    TIM2_Configuration();
}

char Keyboard_GetKey(void)
{
    uint8_t row, col;
    char key = '\0';

    // 扫描行
    for (row = 0; row < ROWS; row++) {
        GPIO_ResetBits(ROW_PORT[row], ROW_PIN[row]);
        delay_us(10); // 延时确保行引脚稳定为低电平

        // 检测列
        for (col = 0; col < COLS; col++) {
            if (GPIO_ReadInputDataBit(COL_PORT[col], COL_PIN[col]) == Bit_RESET) {
                // 按键按下
                // 延时一段时间，以避免按键抖动
                delay_ms(5);

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
    return key;
}

#include "LED_MATRIX.h"

// 定义LED点阵的行和列数
#define ROW_COUNT 4
#define COL_COUNT 4

// 定义行接口的GPIO引脚
#define ROW_GPIO_PORT GPIOA
#define ROW_GPIO_PIN_0 GPIO_Pin_8
#define ROW_GPIO_PIN_1 GPIO_Pin_9
#define ROW_GPIO_PIN_2 GPIO_Pin_10
#define ROW_GPIO_PIN_3 GPIO_Pin_11

// 定义列接口的GPIO引脚
#define COL_GPIO_PORT GPIOB
#define COL_GPIO_PIN_0 GPIO_Pin_12
#define COL_GPIO_PIN_1 GPIO_Pin_13
#define COL_GPIO_PIN_2 GPIO_Pin_14
#define COL_GPIO_PIN_3 GPIO_Pin_15

// 设置行引脚的状态
void set_row(uint8_t row) {
    GPIO_ResetBits(ROW_GPIO_PORT, ROW_GPIO_PIN_0 | ROW_GPIO_PIN_1 | ROW_GPIO_PIN_2 | ROW_GPIO_PIN_3);
    switch (row) {
        case 0:
            GPIO_SetBits(ROW_GPIO_PORT, ROW_GPIO_PIN_0);
            break;
        case 1:
            GPIO_SetBits(ROW_GPIO_PORT, ROW_GPIO_PIN_1);
            break;
        case 2:
            GPIO_SetBits(ROW_GPIO_PORT, ROW_GPIO_PIN_2);
            break;
        case 3:
            GPIO_SetBits(ROW_GPIO_PORT, ROW_GPIO_PIN_3);
            break;
    }
}

// 设置列引脚的状态
void set_col(uint8_t col, uint8_t state) {
    switch (col) {
        case 0:
            GPIO_WriteBit(COL_GPIO_PORT, COL_GPIO_PIN_0, (BitAction)state);
            break;
        case 1:
            GPIO_WriteBit(COL_GPIO_PORT, COL_GPIO_PIN_1, (BitAction)state);
            break;
        case 2:
            GPIO_WriteBit(COL_GPIO_PORT, COL_GPIO_PIN_2, (BitAction)state);
            break;
        case 3:
            GPIO_WriteBit(COL_GPIO_PORT, COL_GPIO_PIN_3, (BitAction)state);
            break;
    }
}

void character_selection(uint8_t arrow_pattern[4])
{
    while(1)
    {
        for (uint8_t row = 0; row < 4; row++)
        {
            set_row(row); // 设置行引脚状态，显示对应的行

            for (uint8_t col = 0; col < 4; col++)
            {
                // 检查当前行和列是否需要亮
                if ((arrow_pattern[row] >> (3 - col)) & 0x01)
                {
                    set_col(col, 1); // 打开当前列，点亮LED
                }
                else
                {
                    set_col(col, 0); // 关闭当前列，熄灭LED
                }
            }
            delay_ms(5);
        }
        if(Keyboard_GetKey() == 'C') return;
    }
}

void displayArrowPattern(void)
{
    OLED_Clear();
    OLED_ShowString(0, 0, "3.", 16);
    OLED_ShowChinese(16, 0, 18, 16); //字
    OLED_ShowChinese(34, 0, 19, 16); //符
    OLED_ShowChinese(52, 0, 20, 16); //选
    OLED_ShowChinese(70, 0, 21, 16); //择

    int isPaused = 0; // 暂停标志位，0表示未暂停，非零值表示暂停状态
    int speed = 15;  // 流水速度，初始值为15
    int prevButtonState = 0;     // 上一次按键状态，0表示未按下，非零值表示按下
    int currentButtonState = 0;

    while (1)
    {
        for (uint8_t row = 0; row < 4; row++)
        {
            set_row(row); // 设置行引脚状态，显示对应的行

            if (!isPaused) // 如果不是暂停状态，则进行左移或右移操作
            {
                if (Keyboard_GetKey() == '7')
                {
                    arrow_pattern[row] = (arrow_pattern[row] >> 1) | (arrow_pattern[row] << 3);
                }
                else
                {
                    arrow_pattern[row] = (arrow_pattern[row] << 1) | (arrow_pattern[row] >> 3);
                }
            }

            for (uint8_t col = 0; col < 4; col++)
            {
                // 检查当前行和列是否需要亮
                if ((arrow_pattern[row] >> (3 - col)) & 0x01)
                {
                    set_col(col, 1); // 打开当前列，点亮LED
                }
                else
                {
                    set_col(col, 0); // 关闭当前列，熄灭LED
                }
            }

            currentButtonState = (Keyboard_GetKey() == '5');

            if (currentButtonState && !prevButtonState)
            {
                isPaused = !isPaused;  // 切换暂停状态
                delay_ms(200);         // 延时一段时间以避免连续按下键
            }

            prevButtonState = currentButtonState;

            if (isPaused)
            {
                while (Keyboard_GetKey() == '5') {
                    // 等待按键释放
                }
            }

            currentButtonState = (Keyboard_GetKey() == 'B');

            if (currentButtonState && !prevButtonState)
            {
                // 减少流水速度
                speed += 5;
                if (speed > 100) {
                    speed = 100;
                }
                // 在OLED上显示流水速度
                OLED_ShowNum(0, 36, speed, 3, 16);
                // 刷新OLED显示
                OLED_Refresh();
                delay_ms(200);         // 延时一段时间以避免连续按下键
            }

            prevButtonState = currentButtonState;

            currentButtonState = (Keyboard_GetKey() == 'A');

            if (currentButtonState && !prevButtonState)
            {
                // 增加流水速度
                speed -= 5;
                if (speed < 5) {
                    speed = 5;
                }
                // 在OLED上显示流水速度
                OLED_ShowNum(0, 36, speed, 3, 16);
                // 刷新OLED显示
                OLED_Refresh();
                delay_ms(200);         // 延时一段时间以避免连续按下键
            }

            prevButtonState = currentButtonState;

            currentButtonState = (Keyboard_GetKey() == 'c');

            if (currentButtonState && !prevButtonState)
            {
                return;  // 返回函数
            }

            prevButtonState = currentButtonState;
        }

        // 短暂延时，控制显示刷新速度
        delay_ms(speed);
    }
}


void GPIO_Config1(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOB的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置行引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置列引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

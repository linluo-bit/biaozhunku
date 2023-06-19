#ifndef __BEEP_H
#define __BEEP_H     
//#include "sys.h"
#include "stm32f1xx_hal.h"
#include "main.h"

#define BEEP_0   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
#define BEEP_1   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
#define delay_ms(time) HAL_Delay(time);

//#define u32  uint32_t;
//#define u16  uint16_t;
//#define u8   uint8_t;

void BEEP_Init(void);        
void Sound(uint16_t frq);
void Sound2(uint16_t time);
void play_music(void);
void play_successful(void);
void play_failed(void);
void alarm_music(void);
void play_warn(void);
#endif

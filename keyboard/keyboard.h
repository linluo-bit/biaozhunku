#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stm32f10x.h"

#define ROWS 4
#define COLS 4

extern GPIO_TypeDef* ROW_PORT[ROWS];
extern const uint16_t ROW_PIN[ROWS];
extern GPIO_TypeDef* COL_PORT[COLS];
extern const uint16_t COL_PIN[COLS];

extern char keymap[ROWS][COLS];

void GPIO_Configuration(void);
void TIM2_Configuration(void);
void Keyboard_Init(void);
char Keyboard_GetKey(void);

#endif

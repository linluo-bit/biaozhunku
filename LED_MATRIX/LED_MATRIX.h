#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include "stm32f10x.h"

void set_row(uint8_t row);
void set_col(uint8_t col, uint8_t state);
void character_selection(uint8_t arrow_pattern[4]);
void displayArrowPattern(void);
void GPIO_Config1(void);

#endif /* LED_MATRIX_H */

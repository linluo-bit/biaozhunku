#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stm32f10x.h"

void GPIO_Configuration(void);
char read_key(void);


//		char key = read_key();
//    if (key != '\0') {
//			OLED_ShowChar(0,0,key,12);
//			OLED_Refresh();
//		}
            // 处理按键逻辑
            // ...

#endif /* KEYBOARD_H */

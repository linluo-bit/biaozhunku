#ifndef  __AT24C02_H
#define  __AT24C02_H
#include "stm32f10x.h"
 
 
#define AT24C02_I2Cx I2C1    //AT24C02所用的iic外设
#define AT24C02_ADDR  0xA0   //设备地址
 
extern void AT24C02_Init(void); //初始化
extern uint8_t AT24C02_ReadOneByte(uint8_t ReadAddr);	//指定地址读取一个字节
extern void AT24C02_WriteOneByte(uint8_t WriteAddr,uint8_t DataToWrite);	//指定地址写入一个字节
extern void AT24C02_Write(uint8_t WriteAddr,uint8_t *Buffer,uint16_t Num);	//从指定地址开始写入指定长度的数据
extern void AT24C02_Read(uint8_t ReadAddr,uint8_t *Buffer,uint16_t Num);   	//从指定地址开始读取指定长度的数据
 
#endif

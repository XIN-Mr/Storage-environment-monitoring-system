#ifndef BH1750_H
#define BH1750_H
#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
//#include "bsp_SysTick.h"
 
//IIC总线地址接口定义
#define BH1750_Addr 0x46
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07


void BH1750_Init(void);  //IO初始化，
void BH1750_Start(void);			//上电，设置清除数据寄存器
void BH1750_Read(void);                               //连续的读取内部寄存器数据
void BH1750_Convert(void);
float BH1750_GetLumen(void);
 
#endif

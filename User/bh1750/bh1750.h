#ifndef _BH1750_H_
#define _BH1750_H_

#include "bsp_SysTick.h"
#include "timer.h"
#include "bsp_usart1.h"
/*
*	连接   VCC-->3.3V      GND-->GND
*	 ADDR----->GND
*  SCL ----->pb6
*	 sda ----->pb5
*/


//IO方向设置
 
#define BH1750_SDA_IN()  {GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=(u32)8<<20;}
#define BH1750_SDA_OUT() {GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=(u32)3<<20;}

//IO操作函数	 
#define BH1750_IIC_SCL    PBout(6) //SCL
#define BH1750_IIC_SDA    PBout(5) //SDA	 
#define BH1750_READ_SDA   PBin(5)  //输入SDA 

#define	  BH1750_SlaveAddress   0x46 //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                              //ALT  ADDRESS引脚接地时地址为0x46，接电源时地址为0xB8

void bh1750_iic_init(void);
void bh1750_iic_start(void);
void bh1750_iic_stop(void);
void bh1750_send_ack(char ack);
char bh1750_recv_ack(void);
void bh1750_send_byte(char dat);
char bh1750_recv_byte(unsigned char ack);

void bh1750_iic_init(void);
void Single_Write_bh1750(unsigned char Reg_Address);
void bh1750_ReadOneTime(void);
void bh1750_ReadContinuous1(void);

void bh1750_Init(void);				/****BH1750初始化****/
u8  getLightLayer(void);	/****BH1750光照强度等级****/
extern float lux;



#endif


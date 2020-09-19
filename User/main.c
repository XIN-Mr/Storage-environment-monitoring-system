/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-01-xx
  * @brief   WF-ESP8266 WiFi模块测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "stm32f10x.h"
#include "test.h"


int main ( void )
{
	/* 初始化 */
	USART_Config() ;                                                             //初始化串口1
	SysTick_Init ();                                                               //配置 SysTick 为 1ms 中断一次 
	
	ESP8266_Init ();                                                               //初始化WiFi模块使用的接口和外设
	DHT11_Init ();
	
	ESP8266_StaTcpClient_UnvarnishTest ();
	
	while ( 1 );
}


/*********************************************END OF FILE**********************/

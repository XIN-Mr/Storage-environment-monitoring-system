#ifndef  __TEST_H
#define	 __TEST_H

#include "stm32f10x.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>

/*
串口通信：串口1、串口3
液晶显示屏使用：外部flash读取字模
定时器：TIM3，PWM输出，
中断  ：SysTick 滴答定时器
通信协议：TCP通信协议（ESP封装好了TCP/IP）、贝壳物联
*/

#include "bsp_esp8266.h"//ESP8266_WiFi模块 板载驱动
#include "bsp_SysTick.h"//滴答定时器 板载驱动
#include "bsp_led.h"	//LED-RGB 板载驱动

#include "bsp_dht11.h"	//DHT11温湿度检测模块驱动

#include "bsp_relay.h"	//四路继电器模块驱动
#include "BH1750.h"		//光照强度采集模块驱动
#include "HC_SR501.h"	//人体红外感应模块驱动

//贝壳物联
#include "millis.h"		//
#include "bigiot.h"		//报文发送函数（建立在贝壳物联通信协议上）
#include "cJSON.h"		//1、将得到的字符串解析成json形式；2、从json中获取键值内容

#include "./usart/bsp_usart.h"		//串口1，用于打印调试信息

//TFT-LCD by spi_flash
#include "./lcd/bsp_ili9341_lcd.h"	
#include "./flash/bsp_spi_flash.h"

/********************************** 外部全局变量 ***************************************/
extern volatile uint8_t ucTcpClosedFlag;


/********************************** 测试函数声明 ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );


/*
	温湿度检测
	光照强度数据采集
	继电器控制
	ESP8266数据收发
	LCD液晶显示
	贝壳物联
	
	****ESP8266_TEST1*****
		DEVICE ID：	14106
		APIKEY：	06040b51e

		温度	13972
		湿度	13971
		光照强度 	13974
		人体感应	13926
		
		//继电器开关k1-加温 16859	   
		//继电器开关k2-制冷 16860
		//继电器开关k3-加湿 12802	 
		//继电器开关k4-降湿 13926
		
*/

#endif


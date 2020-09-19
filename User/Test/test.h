#ifndef  __TEST_H
#define	 __TEST_H

#include "stm32f10x.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>

/*
����ͨ�ţ�����1������3
Һ����ʾ��ʹ�ã��ⲿflash��ȡ��ģ
��ʱ����TIM3��PWM�����
�ж�  ��SysTick �δ�ʱ��
ͨ��Э�飺TCPͨ��Э�飨ESP��װ����TCP/IP������������
*/

#include "bsp_esp8266.h"//ESP8266_WiFiģ�� ��������
#include "bsp_SysTick.h"//�δ�ʱ�� ��������
#include "bsp_led.h"	//LED-RGB ��������

#include "bsp_dht11.h"	//DHT11��ʪ�ȼ��ģ������

#include "bsp_relay.h"	//��·�̵���ģ������
#include "BH1750.h"		//����ǿ�Ȳɼ�ģ������
#include "HC_SR501.h"	//��������Ӧģ������

//��������
#include "millis.h"		//
#include "bigiot.h"		//���ķ��ͺ����������ڱ�������ͨ��Э���ϣ�
#include "cJSON.h"		//1�����õ����ַ���������json��ʽ��2����json�л�ȡ��ֵ����

#include "./usart/bsp_usart.h"		//����1�����ڴ�ӡ������Ϣ

//TFT-LCD by spi_flash
#include "./lcd/bsp_ili9341_lcd.h"	
#include "./flash/bsp_spi_flash.h"

/********************************** �ⲿȫ�ֱ��� ***************************************/
extern volatile uint8_t ucTcpClosedFlag;


/********************************** ���Ժ������� ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );


/*
	��ʪ�ȼ��
	����ǿ�����ݲɼ�
	�̵�������
	ESP8266�����շ�
	LCDҺ����ʾ
	��������
	
	****ESP8266_TEST1*****
		DEVICE ID��	14106
		APIKEY��	06040b51e

		�¶�	13972
		ʪ��	13971
		����ǿ�� 	13974
		�����Ӧ	13926
		
		//�̵�������k1-���� 16859	   
		//�̵�������k2-���� 16860
		//�̵�������k3-��ʪ 12802	 
		//�̵�������k4-��ʪ 13926
		
*/

#endif


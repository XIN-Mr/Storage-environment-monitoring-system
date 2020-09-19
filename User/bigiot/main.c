/**
  ******************************************************************************
  * @file    main.c
  * @author  www.bigiot.net
  * @version V0.1
  * @date    2017-6-9
  * @brief   STM32F103C8T6_ESP01_LED���豸��¼Ϊ������ģʽ��
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "LED.h"
#include "usart1.h"
#include "cJSON.h"
#include "millis.h"

volatile u8 aRxBuffer[1024]={0x00};
volatile u8 RxCounter=0;
volatile u8 ReceiveState=0;

unsigned long lastCheckInTime = 0; 
unsigned long lastCheckStatusTime = 0; 
unsigned long lastSayTime = 0; 
const unsigned long postingInterval = 40000; 
const unsigned long statusInterval = 100000; 

/*�����������������޸�Ϊ�Լ����豸ID��APIKEY���ڱ��������û���������豸���ɻ��*/
char *DEVICEID = "xxx";
char *APIKEY = "xxxxxxxxx";

/*��ʱ����*/
void delay_ms(u16 time)
{
    u16 i = 0;
    while(time--)
    {
        i = 12000;
        while(i--);
    }
}

/*���͵�¼��Ϣ*/
void checkin(void)
{
    printf("{\"M\":\"checkin\",\"ID\":\"%s\",\"K\":\"%s\"}\n", DEVICEID, APIKEY);
}
/*�˳���¼*/
void checkout(void)
{
    printf("{\"M\":\"checkout\",\"ID\":\"%s\",\"K\":\"%s\"}\n", DEVICEID, APIKEY);
}
/*��鵱ǰ��¼״̬*/
void check_status(void)
{
    printf("{\"M\":\"status\"}\n");
}
/*����ָ�Ŀ�굥λ*/
void say(char *toID, char *content)
{
    printf("{\"M\":\"say\",\"ID\":\"%s\",\"C\":\"%s\"}\n", toID, content);
}
/*�ϴ�һ���ӿڵ�ʵʱ����*/
void update1(char *did, char *inputid, float value) {
    printf("{\"M\":\"update\",\"ID\":\"%s\",\"V\":{\"%s\":\"%f\"}}\n", did, inputid, value);
}

/*��CJSON������յ�����Ϣ*/
int processMessage(char *msg) {
    cJSON *jsonObj = cJSON_Parse(msg);
    cJSON *method;
    char *m;
    //json�ַ�������ʧ�ܣ�ֱ���˳�
    if(!jsonObj)
    {
        //uart1.printf("json string wrong!");
        return 0;
    }
    method = cJSON_GetObjectItem(jsonObj, "M");
    m = method->valuestring;
    if(strncmp(m, "WELCOME", 7) == 0)
    {
        //��ֹ�豸����״̬δ�������ȵǳ�
        checkout();
        //��ֹ��������ָ�����
        delay_ms(500);
        checkin();
    }
    if(strncmp(m, "connected", 9) == 0)
    {
        checkout();
        delay_ms(500);
        checkin();
    }
    //���豸���û���¼�����ͻ�ӭ��Ϣ
    if(strncmp(m, "login", 5) == 0)
    {
        char *from_id = cJSON_GetObjectItem(jsonObj, "ID")->valuestring;
        char new_content[] = "Dear friend, welcome to BIGIOT !";
        say(from_id, new_content);
    }
    //�յ�sayָ�ִ����Ӧ��������������Ӧ�ظ�
    if(strncmp(m, "say", 3) == 0 && millis() - lastSayTime > 10)
    {
        char *content = cJSON_GetObjectItem(jsonObj, "C")->valuestring;
        char *from_id = cJSON_GetObjectItem(jsonObj, "ID")->valuestring;
        lastSayTime = millis();
        if(strncmp(content, "play", 4) == 0)
        {
            char new_content[] = "led played";
            //do something here....
            Led_On(LED1);//����LED��
            Led_On(LED2);//���LED��
            say(from_id, new_content);
        }
        else if(strncmp(content, "stop", 4) == 0)
        {
            char new_content[] = "led stoped";
            //do something here....
            Led_Off(LED1);//����LED��
            Led_Off(LED2);//���LED��
            say(from_id, new_content);
        }
    }
    if(jsonObj)cJSON_Delete(jsonObj);
    return 1;
}


void setup(void)
{
    LED_Init();
    USART1_Config();
    MILLIS_Init();
}
int main(void)
{
    u8 i = 0;
    setup();
    while (1)
    {
        //delay_ms(100);
        if (millis() - lastCheckInTime > postingInterval || lastCheckInTime == 0) {
            checkin();
            lastCheckInTime = millis();
        }
        
        if (millis() - lastCheckStatusTime > statusInterval) {
            check_status();
            lastCheckStatusTime = millis();
        }

        if(ReceiveState == 1)
        {
            ReceiveState = 0;
            i = 0;
            processMessage(aRxBuffer);
            RxCounter = 0;
        }
        
    }
}


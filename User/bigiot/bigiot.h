#ifndef __BIGIOT_H
#define	__BIGIOT_H

#include "bsp_esp8266.h"
/*���͵�¼��Ϣ*/
void checkin(char *DEVICEID,char *APIKEY);
/*�˳���¼*/
void checkout(char *DEVICEID,char *APIKEY);
/*��鵱ǰ��¼״̬*/
void check_status(void);
/*����ָ�Ŀ�굥λ*/
void say(char *toID, char *content);
/*�ϴ�һ���ӿڵ�ʵʱ����*/
//void update1(char *did, char *inputid, float value);
//void update2(char *did, char *inputid, uint8_t intV, uint8_t deciV);
void updateMsg(
	char *did, 
	char *inputid1, uint8_t temp_int, uint8_t temp_deci, 
	char *inputid2, uint8_t humi_int, uint8_t humi_deci,
	char *inputid3, float lightLux,
	char *inputid4, uint8_t humanState);
#endif

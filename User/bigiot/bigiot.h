#ifndef __BIGIOT_H
#define	__BIGIOT_H

#include "bsp_esp8266.h"
/*发送登录信息*/
void checkin(char *DEVICEID,char *APIKEY);
/*退出登录*/
void checkout(char *DEVICEID,char *APIKEY);
/*检查当前登录状态*/
void check_status(void);
/*发送指令到目标单位*/
void say(char *toID, char *content);
/*上传一个接口的实时数据*/
//void update1(char *did, char *inputid, float value);
//void update2(char *did, char *inputid, uint8_t intV, uint8_t deciV);
void updateMsg(
	char *did, 
	char *inputid1, uint8_t temp_int, uint8_t temp_deci, 
	char *inputid2, uint8_t humi_int, uint8_t humi_deci,
	char *inputid3, float lightLux,
	char *inputid4, uint8_t humanState);
#endif

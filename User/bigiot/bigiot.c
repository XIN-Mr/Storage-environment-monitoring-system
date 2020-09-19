#include "bigiot.h"

char IOTStr [ 128 ] = { 0 };
/*发送登录信息*/
void checkin(char *DEVICEID,char *APIKEY)
{
	sprintf(IOTStr, "{\"M\":\"checkin\",\"ID\":\"%s\",\"K\":\"%s\"}\n", DEVICEID, APIKEY);
	ESP8266_SendString ( ENABLE, IOTStr, 0, Single_ID_0 );
}
/*退出登录*/
void checkout(char *DEVICEID,char *APIKEY)
{
	sprintf(IOTStr, "{\"M\":\"checkout\",\"ID\":\"%s\",\"K\":\"%s\"}\n", DEVICEID, APIKEY);
	ESP8266_SendString ( ENABLE, IOTStr, 0, Single_ID_0 );
}
/*检查当前登录状态*/
void check_status(void)
{
	sprintf(IOTStr, "{\"M\":\"status\"}\n");
	ESP8266_SendString ( ENABLE, IOTStr, 0, Single_ID_0 );
}
/*发送指令到目标单位*/
void say(char *toID, char *content)
{
	sprintf(IOTStr, "{\"M\":\"say\",\"ID\":\"%s\",\"C\":\"%s\"}\n", toID, content);
	ESP8266_SendString ( ENABLE, IOTStr, 0, Single_ID_0 );
}

/* {"M":"update","ID":"112","V":{"6":"1","36":"116"}}\n */
/*上传多个接口的实时数据，float型数据*/
void updateMsg(
	char *did, 
	char *inputid1, uint8_t temp_int, uint8_t temp_deci, 
	char *inputid2, uint8_t humi_int, uint8_t humi_deci,
	char *inputid3, float lightLux,
	char *inputid4, uint8_t humanState)
{
	sprintf(IOTStr, "{\"M\":\"update\",\"ID\":\"%s\",\"V\":{\"%s\":\"%d.%d\",\"%s\":\"%d.%d\",\"%s\":\"%f\", \"%s\":\"%d\"}}\n",	
	did, inputid1, temp_int,temp_deci,inputid2,humi_int,humi_deci, inputid3, lightLux,inputid4,humanState);//{"M":"update","ID":"2","V":{"2":"120"}}\n
	ESP8266_SendString ( ENABLE, IOTStr, 0, Single_ID_0 );
}

//void update2(char *did, char *inputid, float TEMP_value)	// 上传温度数据/湿度数据
//{
//	sprintf(IOTStr, "{\"M\":\"update\",\"ID\":\"%s\",\"V\":{\"%s\":\"%d.%d\"}}\n", did, inputid, intV, deciV);//{"M":"update","ID":"2","V":{"2":"120"}}\n
//	ESP8266_SendString ( ENABLE, IOTStr, 0, Single_ID_0 );
//}

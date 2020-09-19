#include "test.h"

volatile uint8_t ucTcpClosedFlag = 0;

unsigned long lastCheckInTime = 0; 
unsigned long lastCheckStatusTime = 0; 
unsigned long lastSayTime = 0; 
const unsigned long postingInterval = 40000; 
const unsigned long statusInterval = 100000; 

/*以下两个变量必须修改为自己的设备ID及APIKEY，在贝壳物联用户中心添加设备即可获得*/
char *DEVICEID = "14106";
char *APIKEY = "06040b51e";

char *TEMP=	"13972";
char *HUMI=	"13971";
char *LIGHT="13974";
char *HUMAN="13926";

void loginIOT(void);
int processMessage(char *msg);
void transmitData(char *cStr);
void receiveData(void);
void reconnectWiFiTCP(uint8_t *ucStatus);
//void TFT_LCD_Init(void);

void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	char cStr [ 100 ] = { 0 };
	char *temp = 0;
	
	
	printf ( "\r\n正在配置 ESP8266 ......\r\n" );
	
	WiFiConfig_STA();
	LED_GPIO_Config();
	MILLIS_Init();
	RELAY_GPIO_Config();
	bh1750_Init();
	HC_SR501_GPIO_Config();
	//LCD 初始化
	ILI9341_Init ();         
	ILI9341_GramScan ( 6 );
	
	/*****LCD界面标题初始化********/
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);/* 清屏，显示全黑 */
	/********显示字符串示例*******/ 
	temp = "仓储环境监测系统";
	sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(temp))/2,' ',temp);	/*使用c标准库把变量转化成字符串*/
	LCD_ClearLine(LINEY(2));	/* 清除单行文字 */	
	ILI9341_DispStringLine_EN_CH(LINE(2),cStr);
	printf ( "\r\n TFT-LCD初始化完成！\r\n" );
	LCD_SetFont(&Font8x16);
	while(1)
	{
		loginIOT();
		//Delay_ms(100);
		transmitData(cStr);		//传输数据、显示、判断处理
		receiveData();			//接收数据
		reconnectWiFiTCP(&ucStatus);	//TCP服务器重连机制
	}
}

void transmitData(char *cStr)
{
	DHT11_Data_TypeDef DHT11_Data;
	char dispBuff[100] ={0};
	char *human_status =0;
	if(SR501_Dout_IN())
		human_status = "有人";
	else
		human_status = "无人";
	getValue();
	if ( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS )       //读取 DHT11 温湿度信息  光照层级：%d
		sprintf ( cStr, "\r\n读取数据成功!\r\n\r\n湿度为%d.%d ％ ，温度为 %d.%d℃  \r\n光照强度：%f lux \r\n 仓储室人体感应：%s\r\n", 
							DHT11_Data .humi_int, DHT11_Data .humi_deci, DHT11_Data .temp_int, DHT11_Data.temp_deci, lux ,human_status);
	else
		sprintf ( cStr, "Read DHT11 ERROR!\r\n" );
	printf ( "%s", cStr ); 	//打印读取到的温湿度、光照强度信息                                         
//	ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //发送 DHT11 温湿度信息到网络调试助手
	
	LCD_SetFont(&Font8x16);
	LCD_SetTextColor(CYAN);
	sprintf(dispBuff,"温度：%d.%d ℃",DHT11_Data .temp_int, DHT11_Data .temp_deci);	/*使用c标准库把变量转化成字符串*/
	sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(dispBuff))/2,' ',dispBuff);	/*使用c标准库把变量转化成字符串*/
	LCD_ClearLine(LINE(6));	/* 清除单行文字 */	
	ILI9341_DispStringLine_EN_CH(LINE(6),cStr);/*然后显示该字符串即可，其它变量也是这样处理*/
	
	LCD_SetTextColor(GREEN);
	sprintf(dispBuff,"湿度：%d.%d %%",DHT11_Data .humi_int, DHT11_Data .humi_deci);	/*使用c标准库把变量转化成字符串*/
	sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(dispBuff))/2,' ',dispBuff);	/*使用c标准库把变量转化成字符串*/
	LCD_ClearLine(LINE(8));	/* 清除单行文字 */	
	ILI9341_DispStringLine_EN_CH(LINE(8),cStr);/*然后显示该字符串即可，其它变量也是这样处理*/

	LCD_SetTextColor(YELLOW);
	sprintf(dispBuff,"光照度：%.1f Lux",lux);	/*使用c标准库把变量转化成字符串*/
	sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(dispBuff))/2,' ',dispBuff);	/*使用c标准库把变量转化成字符串*/
	LCD_ClearLine(LINE(10));	/* 清除单行文字 */	
	ILI9341_DispStringLine_EN_CH(LINE(10),cStr);/*然后显示该字符串即可，其它变量也是这样处理*/
	
	LCD_SetTextColor(MAGENTA);
	sprintf(dispBuff,"人体红外检测：%s",human_status);	/*使用c标准库把变量转化成字符串*/
	sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(dispBuff))/2,' ',dispBuff);	/*使用c标准库把变量转化成字符串*/
	LCD_ClearLine(LINE(12));	/* 清除单行文字 */	
	ILI9341_DispStringLine_EN_CH(LINE(12),cStr);/*然后显示该字符串即可，其它变量也是这样处理*/

/**************温度参数自动控制核心代码************/
	updateMsg(
		DEVICEID, 
		TEMP, DHT11_Data .temp_int, DHT11_Data.temp_deci,
		HUMI, DHT11_Data .humi_int, DHT11_Data .humi_deci,
		LIGHT, lux,
		HUMAN, SR501_Dout_IN()
	);

	/***********	TEMP:	20℃-28℃	*************/
	if(DHT11_Data .temp_int>=20 && DHT11_Data .temp_int<=28)
	{
		printf("仓储环境温度正常，关闭加温、制冷器\r\n");
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("加温器关闭，制冷器关闭"))/2,'-',"加温器关闭，制冷器关闭");	/*使用c标准库把变量转化成字符串*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(16));	/* 清除单行文字 */	
		ILI9341_DispStringLine_EN_CH(LINE(16),cStr);
		RELAY1_OFF
		RELAY2_OFF
	}
	else if(DHT11_Data .temp_int < 20)
	{
		printf("仓储环境温度过低，开始加温...\r\n");
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("加温器状态：开启"))/2,'-',"加温器状态：开启");	/*使用c标准库把变量转化成字符串*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(16));	/* 清除单行文字 */	
		ILI9341_DispStringLine_EN_CH(LINE(16),cStr);
		RELAY2_OFF
		RELAY1_ON
	}
	else if(DHT11_Data .temp_int > 28)
	{
		printf("仓储环境温度过高，开始制冷...\r\n");
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("制冷器状态：开启"))/2,'-',"制冷器状态：开启");	/*使用c标准库把变量转化成字符串*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(16));	/* 清除单行文字 */	
		ILI9341_DispStringLine_EN_CH(LINE(16),cStr);
		RELAY1_OFF
		RELAY2_ON
	}
	/***********	HUMI: 	20%-85%		*************/
	
	if(DHT11_Data .humi_int>=20 && DHT11_Data .humi_int<=85)
	{
		printf("仓储环境湿度正常，关闭加湿、抽湿器\r\n");
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("加湿器关闭，抽湿器关闭"))/2,'-',"加湿器关闭，抽湿器关闭");	/*使用c标准库把变量转化成字符串*/
		RELAY3_OFF
		RELAY4_OFF
		
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(17));	/* 清除单行文字 */	
		ILI9341_DispStringLine_EN_CH(LINE(17),cStr);	
	}
	else if(DHT11_Data .humi_int < 20)
	{
		printf("仓储环境湿度过低，开始加湿···\r\n");
		RELAY4_OFF
		RELAY3_ON
		
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("加湿器状态：开启"))/2,'-',"加湿器状态：开启");	/*使用c标准库把变量转化成字符串*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(17));	/* 清除单行文字 */	
		ILI9341_DispStringLine_EN_CH(LINE(17),cStr);
	}
	else if(DHT11_Data .humi_int > 85)
	{
		printf("仓储环境湿度过高，开始抽湿···\r\n");
		
		
		RELAY3_OFF
		RELAY4_ON
		
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("抽湿器状态：开启"))/2,'-',"抽湿器状态：开启");	/*使用c标准库把变量转化成字符串*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(17));	/* 清除单行文字 */	
		ILI9341_DispStringLine_EN_CH(LINE(17),cStr);
	}
		
	/***********	LIGHT: 	30 ~ 75 lux	*************/
	if(lux >=30 && lux <=75)
	{
		printf("环境光照度正常\r\n");
		LED_GREEN
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("环境光照度正常"))/2,'-',"环境光照度正常");	/*使用c标准库把变量转化成字符串*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(18));	/* 清除单行文字 */	
		ILI9341_DispStringLine_EN_CH(LINE(18),cStr);
	}
	else if(lux < 30)
	{
		printf("仓储环境光照过弱\r\n");
		LED_WHITE
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("仓储环境光照过弱"))/2,'-',"仓储环境光照过弱");	/*使用c标准库把变量转化成字符串*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(18));	/* 清除单行文字 */	
		ILI9341_DispStringLine_EN_CH(LINE(18),cStr);
		
	}
	else if(lux > 75)
	{
		printf("仓储环境光照过强\r\n");
		LED_RED
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("仓储环境光照过强"))/2,'-',"仓储环境光照过强");	/*使用c标准库把变量转化成字符串*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(YELLOW);
		LCD_ClearLine(LINE(18));	/* 清除单行文字 */	
		ILI9341_DispStringLine_EN_CH(LINE(18),cStr);
	}
}

void reconnectWiFiTCP(uint8_t *ucStatus)
{
	if ( ucTcpClosedFlag )                                             //检测是否失去连接
	{
		ESP8266_ExitUnvarnishSend ();                                    //退出透传模式
		
		do *ucStatus = ESP8266_Get_LinkStatus ();                         //获取连接状态
		while ( ! *ucStatus );
		
		if ( *ucStatus == 4 )                                             //确认失去连接后重连
		{
			printf ( "\r\n正在重连热点和服务器 ......\r\n" );
			
			while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
			
			while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
			
			printf ( "\r\n重连热点和服务器成功\r\n" );
		}
		while ( ! ESP8266_UnvarnishSend ());		
	}
}

void loginIOT(void)	//连接 bigIoT
{
	//delay_ms(100);
	if (millis() - lastCheckInTime > postingInterval || lastCheckInTime == 0) {
		checkin(DEVICEID,APIKEY);
		lastCheckInTime = millis();
	}
	
	if (millis() - lastCheckStatusTime > statusInterval) {
		check_status();
		lastCheckStatusTime = millis();
	}
}

void receiveData()
{
	if ( strEsp8266_Fram_Record .InfBit .FramFinishFlag )
	{
		USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
		
		strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
		
//		printf ( "\r\n%s\r\n", strEsp8266_Fram_Record .Data_RX_BUF );//

		processMessage(strEsp8266_Fram_Record .Data_RX_BUF);
		strEsp8266_Fram_Record .InfBit .FramLength = 0;
		strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
		
		USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断
	}
}

/*用CJSON处理接收到的信息*/
int processMessage(char *msg)
{
    cJSON *jsonObj = cJSON_Parse(msg);
    cJSON *method;
    char *m;
    //json字符串解析失败，直接退出
    if(!jsonObj)
    {
        //uart1.printf("json string wrong!");
        return 0;
    }
    method = cJSON_GetObjectItem(jsonObj, "M");
    m = method->valuestring;
    if(strncmp(m, "WELCOME", 7) == 0)
    {
        //防止设备在线状态未消除，先登出
        checkout(DEVICEID,APIKEY);
        //防止连续发送指令过快
        Delay_ms(500);
        checkin(DEVICEID,APIKEY);
    }
    if(strncmp(m, "connected", 9) == 0)
    {
        checkout(DEVICEID,APIKEY);
        Delay_ms(500);
        checkin(DEVICEID,APIKEY);
    }
    //有设备或用户登录，发送欢迎信息
    if(strncmp(m, "login", 5) == 0)
    {
        char *from_id = cJSON_GetObjectItem(jsonObj, "ID")->valuestring;
        char new_content[] = "Dear friend, welcome to BIGIOT !";
        say(from_id, new_content);
		printf("%s/r/n",new_content);
		
    }
    //收到say指令，执行相应动作，并进行相应回复
    if(strncmp(m, "say", 3) == 0 && millis() - lastSayTime > 10)
    {
        char *content = cJSON_GetObjectItem(jsonObj, "C")->valuestring;
        char *from_id = cJSON_GetObjectItem(jsonObj, "ID")->valuestring;
        lastSayTime = millis();
        if(strncmp(content, "T+S",3) == 0)	//---S1
        {
            char new_content[] = "k1--warming_k1 started";
            //do something here....
            RELAY1_ON
			Delay_ms(1000);
			printf("%s/r/n",new_content);
            say(from_id, new_content);
        }
        else if(strncmp(content, "T+P",3) == 0)
        {
            char new_content[] = "k1--warming_k1 paused";
            //do something here....
			RELAY1_OFF
			Delay_ms(1000);
			printf("%s/r/n",new_content);
            say(from_id, new_content);
        }
		
		if(strncmp(content, "T-S", 3) == 0)	//---S2
        {
            char new_content[] = "k2--cooling_switch started";
            //do something here....
            RELAY2_ON
			Delay_ms(1000);
			printf("%s/r/n",new_content);
            say(from_id, new_content);
        }
        else if(strncmp(content, "T-P", 3) == 0)	
        {
            char new_content[] = "k2--cooling_switch paused";
            //do something here....
			RELAY2_OFF
			Delay_ms(1000);
			printf("%s/r/n",new_content);
            say(from_id, new_content);
        }
		
		if(strncmp(content, "H+S",3) == 0)	//---S3
        {
            char new_content[] = "k3--humier_switch started";
            //do something here....
            RELAY3_ON
			Delay_ms(1000);
			printf("%s/r/n",new_content);
            say(from_id, new_content);
        }
        else if(strncmp(content,  "H+P", 3) == 0)
        {
            char new_content[] = "k3--humier_switch paused";
            //do something here....
			RELAY3_OFF
			Delay_ms(1000);
			printf("%s/r/n",new_content);
            say(from_id, new_content);
        }
		if(strncmp(content,  "H-S", 3) == 0)	//---S4
        {
            char new_content[] = "k4--dehumi_switch started";
            //do something here....
            RELAY4_ON
			Delay_ms(1000);
			printf("%s/r/n",new_content);
            say(from_id, new_content);
        }
        else if(strncmp(content, "H-P", 3) == 0) 
        {
            char new_content[] = "k4--dehumi_switch paused";
            //do something here....
			RELAY4_OFF
			Delay_ms(1000);
			printf("%s/r/n",new_content);
            say(from_id, new_content);
        }
    }
    if(jsonObj)cJSON_Delete(jsonObj);
    return 1;
}

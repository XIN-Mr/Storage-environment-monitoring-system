#include "test.h"

volatile uint8_t ucTcpClosedFlag = 0;

unsigned long lastCheckInTime = 0; 
unsigned long lastCheckStatusTime = 0; 
unsigned long lastSayTime = 0; 
const unsigned long postingInterval = 40000; 
const unsigned long statusInterval = 100000; 

/*�����������������޸�Ϊ�Լ����豸ID��APIKEY���ڱ��������û���������豸���ɻ��*/
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
	
	
	printf ( "\r\n�������� ESP8266 ......\r\n" );
	
	WiFiConfig_STA();
	LED_GPIO_Config();
	MILLIS_Init();
	RELAY_GPIO_Config();
	bh1750_Init();
	HC_SR501_GPIO_Config();
	//LCD ��ʼ��
	ILI9341_Init ();         
	ILI9341_GramScan ( 6 );
	
	/*****LCD��������ʼ��********/
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);/* ��������ʾȫ�� */
	/********��ʾ�ַ���ʾ��*******/ 
	temp = "�ִ��������ϵͳ";
	sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(temp))/2,' ',temp);	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	LCD_ClearLine(LINEY(2));	/* ����������� */	
	ILI9341_DispStringLine_EN_CH(LINE(2),cStr);
	printf ( "\r\n TFT-LCD��ʼ����ɣ�\r\n" );
	LCD_SetFont(&Font8x16);
	while(1)
	{
		loginIOT();
		//Delay_ms(100);
		transmitData(cStr);		//�������ݡ���ʾ���жϴ���
		receiveData();			//��������
		reconnectWiFiTCP(&ucStatus);	//TCP��������������
	}
}

void transmitData(char *cStr)
{
	DHT11_Data_TypeDef DHT11_Data;
	char dispBuff[100] ={0};
	char *human_status =0;
	if(SR501_Dout_IN())
		human_status = "����";
	else
		human_status = "����";
	getValue();
	if ( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS )       //��ȡ DHT11 ��ʪ����Ϣ  ���ղ㼶��%d
		sprintf ( cStr, "\r\n��ȡ���ݳɹ�!\r\n\r\nʪ��Ϊ%d.%d �� ���¶�Ϊ %d.%d��  \r\n����ǿ�ȣ�%f lux \r\n �ִ��������Ӧ��%s\r\n", 
							DHT11_Data .humi_int, DHT11_Data .humi_deci, DHT11_Data .temp_int, DHT11_Data.temp_deci, lux ,human_status);
	else
		sprintf ( cStr, "Read DHT11 ERROR!\r\n" );
	printf ( "%s", cStr ); 	//��ӡ��ȡ������ʪ�ȡ�����ǿ����Ϣ                                         
//	ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //���� DHT11 ��ʪ����Ϣ�������������
	
	LCD_SetFont(&Font8x16);
	LCD_SetTextColor(CYAN);
	sprintf(dispBuff,"�¶ȣ�%d.%d ��",DHT11_Data .temp_int, DHT11_Data .temp_deci);	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(dispBuff))/2,' ',dispBuff);	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	LCD_ClearLine(LINE(6));	/* ����������� */	
	ILI9341_DispStringLine_EN_CH(LINE(6),cStr);/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
	
	LCD_SetTextColor(GREEN);
	sprintf(dispBuff,"ʪ�ȣ�%d.%d %%",DHT11_Data .humi_int, DHT11_Data .humi_deci);	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(dispBuff))/2,' ',dispBuff);	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	LCD_ClearLine(LINE(8));	/* ����������� */	
	ILI9341_DispStringLine_EN_CH(LINE(8),cStr);/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/

	LCD_SetTextColor(YELLOW);
	sprintf(dispBuff,"���նȣ�%.1f Lux",lux);	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(dispBuff))/2,' ',dispBuff);	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	LCD_ClearLine(LINE(10));	/* ����������� */	
	ILI9341_DispStringLine_EN_CH(LINE(10),cStr);/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/
	
	LCD_SetTextColor(MAGENTA);
	sprintf(dispBuff,"��������⣺%s",human_status);	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(dispBuff))/2,' ',dispBuff);	/*ʹ��c��׼��ѱ���ת�����ַ���*/
	LCD_ClearLine(LINE(12));	/* ����������� */	
	ILI9341_DispStringLine_EN_CH(LINE(12),cStr);/*Ȼ����ʾ���ַ������ɣ���������Ҳ����������*/

/**************�¶Ȳ����Զ����ƺ��Ĵ���************/
	updateMsg(
		DEVICEID, 
		TEMP, DHT11_Data .temp_int, DHT11_Data.temp_deci,
		HUMI, DHT11_Data .humi_int, DHT11_Data .humi_deci,
		LIGHT, lux,
		HUMAN, SR501_Dout_IN()
	);

	/***********	TEMP:	20��-28��	*************/
	if(DHT11_Data .temp_int>=20 && DHT11_Data .temp_int<=28)
	{
		printf("�ִ������¶��������رռ��¡�������\r\n");
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("�������رգ��������ر�"))/2,'-',"�������رգ��������ر�");	/*ʹ��c��׼��ѱ���ת�����ַ���*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(16));	/* ����������� */	
		ILI9341_DispStringLine_EN_CH(LINE(16),cStr);
		RELAY1_OFF
		RELAY2_OFF
	}
	else if(DHT11_Data .temp_int < 20)
	{
		printf("�ִ������¶ȹ��ͣ���ʼ����...\r\n");
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("������״̬������"))/2,'-',"������״̬������");	/*ʹ��c��׼��ѱ���ת�����ַ���*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(16));	/* ����������� */	
		ILI9341_DispStringLine_EN_CH(LINE(16),cStr);
		RELAY2_OFF
		RELAY1_ON
	}
	else if(DHT11_Data .temp_int > 28)
	{
		printf("�ִ������¶ȹ��ߣ���ʼ����...\r\n");
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("������״̬������"))/2,'-',"������״̬������");	/*ʹ��c��׼��ѱ���ת�����ַ���*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(16));	/* ����������� */	
		ILI9341_DispStringLine_EN_CH(LINE(16),cStr);
		RELAY1_OFF
		RELAY2_ON
	}
	/***********	HUMI: 	20%-85%		*************/
	
	if(DHT11_Data .humi_int>=20 && DHT11_Data .humi_int<=85)
	{
		printf("�ִ�����ʪ���������رռ�ʪ����ʪ��\r\n");
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("��ʪ���رգ���ʪ���ر�"))/2,'-',"��ʪ���رգ���ʪ���ر�");	/*ʹ��c��׼��ѱ���ת�����ַ���*/
		RELAY3_OFF
		RELAY4_OFF
		
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(17));	/* ����������� */	
		ILI9341_DispStringLine_EN_CH(LINE(17),cStr);	
	}
	else if(DHT11_Data .humi_int < 20)
	{
		printf("�ִ�����ʪ�ȹ��ͣ���ʼ��ʪ������\r\n");
		RELAY4_OFF
		RELAY3_ON
		
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("��ʪ��״̬������"))/2,'-',"��ʪ��״̬������");	/*ʹ��c��׼��ѱ���ת�����ַ���*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(17));	/* ����������� */	
		ILI9341_DispStringLine_EN_CH(LINE(17),cStr);
	}
	else if(DHT11_Data .humi_int > 85)
	{
		printf("�ִ�����ʪ�ȹ��ߣ���ʼ��ʪ������\r\n");
		
		
		RELAY3_OFF
		RELAY4_ON
		
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("��ʪ��״̬������"))/2,'-',"��ʪ��״̬������");	/*ʹ��c��׼��ѱ���ת�����ַ���*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(17));	/* ����������� */	
		ILI9341_DispStringLine_EN_CH(LINE(17),cStr);
	}
		
	/***********	LIGHT: 	30 ~ 75 lux	*************/
	if(lux >=30 && lux <=75)
	{
		printf("�������ն�����\r\n");
		LED_GREEN
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("�������ն�����"))/2,'-',"�������ն�����");	/*ʹ��c��׼��ѱ���ת�����ַ���*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(18));	/* ����������� */	
		ILI9341_DispStringLine_EN_CH(LINE(18),cStr);
	}
	else if(lux < 30)
	{
		printf("�ִ��������չ���\r\n");
		LED_WHITE
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("�ִ��������չ���"))/2,'-',"�ִ��������չ���");	/*ʹ��c��׼��ѱ���ת�����ַ���*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(WHITE);
		LCD_ClearLine(LINE(18));	/* ����������� */	
		ILI9341_DispStringLine_EN_CH(LINE(18),cStr);
		
	}
	else if(lux > 75)
	{
		printf("�ִ��������չ�ǿ\r\n");
		LED_RED
		sprintf(cStr,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen("�ִ��������չ�ǿ"))/2,'-',"�ִ��������չ�ǿ");	/*ʹ��c��׼��ѱ���ת�����ַ���*/
		LCD_SetFont(&Font8x16);
		LCD_SetTextColor(YELLOW);
		LCD_ClearLine(LINE(18));	/* ����������� */	
		ILI9341_DispStringLine_EN_CH(LINE(18),cStr);
	}
}

void reconnectWiFiTCP(uint8_t *ucStatus)
{
	if ( ucTcpClosedFlag )                                             //����Ƿ�ʧȥ����
	{
		ESP8266_ExitUnvarnishSend ();                                    //�˳�͸��ģʽ
		
		do *ucStatus = ESP8266_Get_LinkStatus ();                         //��ȡ����״̬
		while ( ! *ucStatus );
		
		if ( *ucStatus == 4 )                                             //ȷ��ʧȥ���Ӻ�����
		{
			printf ( "\r\n���������ȵ�ͷ����� ......\r\n" );
			
			while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
			
			while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
			
			printf ( "\r\n�����ȵ�ͷ������ɹ�\r\n" );
		}
		while ( ! ESP8266_UnvarnishSend ());		
	}
}

void loginIOT(void)	//���� bigIoT
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
		USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, DISABLE ); //���ô��ڽ����ж�
		
		strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
		
//		printf ( "\r\n%s\r\n", strEsp8266_Fram_Record .Data_RX_BUF );//

		processMessage(strEsp8266_Fram_Record .Data_RX_BUF);
		strEsp8266_Fram_Record .InfBit .FramLength = 0;
		strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
		
		USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //ʹ�ܴ��ڽ����ж�
	}
}

/*��CJSON������յ�����Ϣ*/
int processMessage(char *msg)
{
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
        checkout(DEVICEID,APIKEY);
        //��ֹ��������ָ�����
        Delay_ms(500);
        checkin(DEVICEID,APIKEY);
    }
    if(strncmp(m, "connected", 9) == 0)
    {
        checkout(DEVICEID,APIKEY);
        Delay_ms(500);
        checkin(DEVICEID,APIKEY);
    }
    //���豸���û���¼�����ͻ�ӭ��Ϣ
    if(strncmp(m, "login", 5) == 0)
    {
        char *from_id = cJSON_GetObjectItem(jsonObj, "ID")->valuestring;
        char new_content[] = "Dear friend, welcome to BIGIOT !";
        say(from_id, new_content);
		printf("%s/r/n",new_content);
		
    }
    //�յ�sayָ�ִ����Ӧ��������������Ӧ�ظ�
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

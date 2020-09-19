#include "test.h"
#include "bsp_esp8266.h"
#include "bsp_SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "bsp_dht11.h"

volatile uint8_t ucTcpClosedFlag = 0;
char *RecvStr;
/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	
	char cStr [ 100 ] = { 0 };
	DHT11_Data_TypeDef DHT11_Data;
	
	
  printf ( "\r\n正在配置 ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( STA );

  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	
	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\n配置 ESP8266 完毕\r\n" );
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
	
	while(1)
	{		
		if ( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS )       //读取 DHT11 温湿度信息
			sprintf ( cStr, "\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n", 
								DHT11_Data .humi_int, DHT11_Data .humi_deci, DHT11_Data .temp_int, DHT11_Data.temp_deci );
				
		else
			sprintf ( cStr, "Read DHT11 ERROR!\r\n" );

		printf ( "%s", cStr );                                             //打印读取 DHT11 温湿度信息

		ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //发送 DHT11 温湿度信息到网络调试助手
		
		Delay_ms ( 1000 );

		if ( ucTcpClosedFlag )                                             //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend ();                                    //退出透传模式
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //获取连接状态
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //确认失去连接后重连
			{
				printf ( "\r\n正在重连热点和服务器 ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );
			}
			while ( ! ESP8266_UnvarnishSend ());		
		}
		if ( strEsp8266_Fram_Record .InfBit .FramFinishFlag )
		{
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, DISABLE ); //禁用串口接收中断
			
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';
			
			RecvStr = ESP8266_ReceiveString( DISABLE);
			printf ( "\r\n%s\r\n", strEsp8266_Fram_Record .Data_RX_BUF );//

			strEsp8266_Fram_Record .InfBit .FramLength = 0;
			strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;	
			
			USART_ITConfig ( macESP8266_USARTx, USART_IT_RXNE, ENABLE ); //使能串口接收中断
		}
		
	}		
}









    ui->setupUi(this);
 
    tcpServer = new QTcpServer(this);
    if( !tcpServer->listen(QHostAddress::LocalHost, 13888) )
    {   //监听本地主机的13888端口，如果出错就输出错误信息，并关闭
        qDebug() << tcpServer->errorString();
        close();
    }
    //连接信号和相应槽函数
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(sendMessages()));
 
void Widget::sendMessages()
{
    QByteArray block;   //用于暂存我们要发送的数据
    QDataStream out(&block, QIODevice::WriteOnly);  //使用数据流写入数据
    out.setVersion(QDataStream::Qt_4_7);
    out << (quint16) 0;
    out << tr("hello tcp");
    out.device()->seek(0);
    out << (quint16) (block.size() - sizeof(quint16));
 
    //获取已经建立的连接的子套接字
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
    /*clientConnection->disconnectFromHost();它表示当发送完成时就会断开连接，
     *这时就会发出disconnected()信号，
     *而最后调用deleteLater()函数保证在关闭连接后删除该套接字clientConnection。
    */
    clientConnection->write(block);
    clientConnection->disconnectFromHost();
    ui->statusLabel->setText("Send message successfil");
}



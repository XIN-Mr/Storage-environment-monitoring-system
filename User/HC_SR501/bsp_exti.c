/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./sr501/bsp_exti.h"

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
//static void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* ����NVICΪ���ȼ���1 */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  
//  /* �����ж�Դ������1 */
//  NVIC_InitStructure.NVIC_IRQChannel = SR501_INT_EXTI_IRQ;
//  /* ������ռ���ȼ���1 */
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  /* ���������ȼ���1 */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  /* ʹ���ж�ͨ�� */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//}

 /**
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_SR501_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
//	EXTI_InitTypeDef EXTI_InitStructure;
  
	/*��������GPIO�ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(SR501_INT_GPIO_CLK ,ENABLE);
  
  /* ʹ�� SYSCFG ʱ�� ��ʹ��GPIO�ⲿ�ж�ʱ����ʹ��SYSCFGʱ��*/
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
//  /* ���� NVIC */
//  NVIC_Configuration();
  
	/* ѡ�񰴼�1������ */ 
  GPIO_InitStructure.GPIO_Pin = SR501_INT_GPIO_PIN;
  /* ��������Ϊ����ģʽ */ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	    		
  /* �������Ų�����Ҳ������ */
 // GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  /* ʹ������Ľṹ���ʼ������ */
  GPIO_Init(SR501_INT_GPIO_PORT, &GPIO_InitStructure); 

//	/* ���� EXTI �ж�Դ ��SR501���� */
//  SYSCFG_EXTILineConfig(SR501_INT_EXTI_PORTSOURCE,SR501_INT_EXTI_PINSOURCE);

//  /* ѡ�� EXTI �ж�Դ */
//  EXTI_InitStructure.EXTI_Line = SR501_INT_EXTI_LINE;
//  /* �ж�ģʽ */
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  /* �½��ش��� */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
//  /* ʹ���ж�/�¼��� */
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);

}
/*********************************************END OF FILE**********************/
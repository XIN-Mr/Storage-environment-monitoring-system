#include "HC_SR501.h"


void HC_SR501_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	/*��������GPIO�ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(SR501_INT_GPIO_CLK,ENABLE);
												
/*--------------------------GPIO����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = SR501_INT_GPIO_PIN;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(SR501_INT_GPIO_PORT, &GPIO_InitStructure);

}

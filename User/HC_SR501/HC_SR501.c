#include "HC_SR501.h"


void HC_SR501_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	/*开启按键GPIO口的时钟*/
	RCC_APB2PeriphClockCmd(SR501_INT_GPIO_CLK,ENABLE);
												
/*--------------------------GPIO配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = SR501_INT_GPIO_PIN;
  /* 配置为下拉输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(SR501_INT_GPIO_PORT, &GPIO_InitStructure);

}

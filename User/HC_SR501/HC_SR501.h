#ifndef __SR501_EXTI_H
#define	__SR501_EXTI_H


#include "stm32f10x.h"


//Òý½Å¶¨Òå
#define SR501_INT_GPIO_PORT         GPIOC
#define SR501_INT_GPIO_CLK          RCC_APB2Periph_GPIOC
#define SR501_INT_GPIO_PIN          GPIO_Pin_7

#define      SR501_Dout_IN()	GPIO_ReadInputDataBit( SR501_INT_GPIO_PORT, SR501_INT_GPIO_PIN ) 




void HC_SR501_GPIO_Config(void);


#endif /* __EXTI_H */

#ifndef __EXTI_H
#define	__EXTI_H

#include "stm32f10x.h"

//Òý½Å¶¨Òå
/*******************************************************/
#define SR501_INT_GPIO_PORT                GPIOD
#define SR501_INT_GPIO_CLK                 RCC_APB2Periph_GPIOD
#define SR501_INT_GPIO_PIN                 GPIO_Pin_2
#define SR501_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOD
#define SR501_INT_EXTI_PINSOURCE           EXTI_PinSource2
#define SR501_INT_EXTI_LINE                EXTI_Line12
#define SR501_INT_EXTI_IRQ                 EXTI2_IRQn

#define SR501_IRQHandler                   EXTI2_IRQHandler

/*******************************************************/


void EXTI_SR501_Config(void);

#endif /* __EXTI_H */

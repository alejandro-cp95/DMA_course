/*
 * stm32f3xx_it.c
 *
 *  Created on: 05/08/2020
 *      Author: -_Alex_-
 */

#include "stm32f30x.h"

#define is_it_HT() (DMA1->ISR&(1<<26))
#define is_it_FT() (DMA1->ISR&(1<<25))
#define is_it_TE() (DMA1->ISR&(1<<27))

extern void HT_Complete_callback(void);
extern void FT_Complete_callback(void);
extern void TE_Error_callback(void);

void clear_exti_pending_bit()
{
	EXTI_TypeDef* pEXTI=EXTI;

	if(pEXTI->PR & (1<<13))
	{
		pEXTI->PR |= (1<<13);
	}
}

void EXTI15_10_IRQHandler(void)
{
	USART_TypeDef* pUSART2=USART2;
	// Here we will send UART2_TX DMA request to DMA1 controller
	pUSART2->CR3 |= (1<<7);

	clear_exti_pending_bit();
}

// IRQ handler for DMA Channel 7 global interrupt

void DMA1_Channel7_IRQHandler(void)
{
	DMA_TypeDef* pDMA=DMA1;
	if(is_it_HT())
	{
		HT_Complete_callback();
		pDMA->IFCR |= (1<<26);
	}
	else if(is_it_FT())
	{
		FT_Complete_callback();
		pDMA->IFCR |= (1<<25);
	}
	else if(is_it_TE())
	{
		TE_Error_callback();
		pDMA->IFCR |= (1<<27);
	}
	else
	{

	}
}

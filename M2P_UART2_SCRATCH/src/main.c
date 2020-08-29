/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f30x.h"
#include "stm32f3xx_nucleo.h"

void button_inti(void);
void uart2_init(void);
void dma_init(void);
void send_some_data(void);
void enable_DMA_channel7(void);
void dma_interrupt_configuration(void);
void HT_Complete_callback(void);
void FT_Complete_callback(void);
void TE_Error_callback(void);

#define BASE_ADDR_OF_GPIOC_PERI GPIOC

char dataStream[] ="Hello World\r\n";

int main(void)
{
	RCC_DeInit();
	SystemCoreClockUpdate();

	button_inti();
	uart2_init();
	dma_init();
	dma_interrupt_configuration();
	enable_DMA_channel7();

	for(;;);
}

void send_some_data(void)
{
	char somedata[] ="Hello World\r\n";
	USART_TypeDef* pUSART2=USART2;

	for(uint8_t i=0;i<sizeof(somedata);i++)
	{
		// Make sure that the TXE in the status register is set (TDR is empty)
		while(!(pUSART2->ISR & (1<<7)));
		pUSART2->TDR =somedata[i];
	}

}

void button_inti(void)
{
	// Button is connected to PC13

	GPIO_TypeDef* pGPIOC=BASE_ADDR_OF_GPIOC_PERI;

	RCC_TypeDef* pRCC=RCC;

	EXTI_TypeDef* pEXTI=EXTI;

	SYSCFG_TypeDef* pSYSCFG=SYSCFG;

	// 1. Enable the clock for the GPIOC peripheral
	pRCC->AHBENR |= (1<<19);

	// 2. Keep the GPIO pin in input mode
	pGPIOC->MODER &= ~(0x3<<26);

	// 3. Configure the edge detection on that GPIO pin
	pEXTI->FTSR |= (1<<13);

	// 4. Enable the interrupt over that GPIO pin
	pEXTI->IMR |= (1<<13);

	// 5. Enable the clock for SYSCFG
	pRCC->APB2ENR |= (1<<0);

	// 6. Configuring the SYSCFG CR4 register
	pSYSCFG-> EXTICR[3] |= (0x2<<4);

	// 7. Enable the IRQ related to that GPIO pin in NVIC of the processor
	NVIC_EnableIRQ(EXTI15_10_IRQn);

}
void uart2_init(void)
{
	RCC_TypeDef* pRCC=RCC;
	GPIO_TypeDef* pGPIOA=GPIOA;
	USART_TypeDef* pUSART2=USART2;

	// 1. Enable the peripheral clock for the UART2 peripheral
	pRCC->APB1ENR |= (1<<17);

	// 2. Configure the GPIO pins for UART_TX and UART_RX functionality
		// PA2 as TX and PA3 as RX

		// Let's configure PA2 as UART2_TX

		// 2.1 Enable the cock for the GPIOA peripheral
	pRCC->AHBENR |= (1<<17);

		// 2.2 Let's configure PA2 as UART2_TX and PA3 as UART2_RX
	pGPIOA->MODER &= ~(3<<4);
	pGPIOA->MODER |= (2<<4);
	pGPIOA->AFR[0] &= ~(0xF<<8);
	pGPIOA->AFR[0] |= (7<<8);

	pGPIOA->MODER &= ~(3<<6);
	pGPIOA->MODER |= (2<<6);
	pGPIOA->AFR[0] &= ~(0xF<<12);
	pGPIOA->AFR[0] |= (7<<12);

		// 2.3 Enable or disable pull-up resistor if required
	pGPIOA->PUPDR &= ~(3<<4);
	pGPIOA->PUPDR |= (1<<4);
	pGPIOA->PUPDR &= ~(3<<6);
	pGPIOA->PUPDR |= (1<<6);

	// 3. Configure the baudrate
	pUSART2->BRR=0;
	pUSART2->BRR |= 69;

	// 4. Configure the data width, no. of stop bits, etc

	// 5. Enable the TX engine of the UART peripheral
	pUSART2->CR1 |= (1<<3);

	// 6. Enable the UART peripheral
	pUSART2->CR1 |= 1;

}
void dma_init(void)
{
	RCC_TypeDef* pRCC=RCC;
	DMA_Channel_TypeDef* pDMAC7=DMA1_Channel7;
	USART_TypeDef* pUSART2=USART2;

	// 1. Enable the peripheral clock for the DMA1
	pRCC->AHBENR |= 1;

	// 2. Identify the stream which is suitable for your peripheral
/////////////////

	// 3. Identify the channel number on which UART2 peripheral sends DMA requests
	// Channel 7

	// 4. Program the source address (memory)
	pDMAC7->CMAR = (uint32_t)dataStream;

	// 5. Program the destination address (peripheral)
	pDMAC7->CPAR = (uint32_t)&pUSART2->TDR;

	// 6. Program number of data items to send
	pDMAC7->CNDTR = sizeof(dataStream);

	// 7. The direction of data transfer M2P, P2M, M2M
	pDMAC7->CCR |= (1<<4);
// Falta el MINC PINC

	// 8. Program the source and destination data width
	pDMAC7->CCR &= ~(3<<8);
	pDMAC7->CCR &= ~(3<<10);

	// 9. Enable memory auto increment
	pDMAC7->CCR |= (1<<7);

	// 10. Select direct mode or FIFO mode
// No necesario para este micro

	// 11. Select the FIFO threshold
// No necesario para este micro

	// 12. Enable the circular mode if required

	// 13. Single transfer or burst transfer

	// 14. Configure the stream priority

}

void dma_interrupt_configuration(void)
{
	DMA_Channel_TypeDef* pDMAC7=DMA1_Channel7;

	// 1. Enable complete-transfer interrupt (TCIE)
	pDMAC7->CCR |= (1<<1);

	// 2. Enable half-transfer interrupt (HTIE)
	pDMAC7->CCR |= (1<<2);

	// 3. Enable transfer error interrupt (TEIE)
	pDMAC7->CCR |= (1<<3);

	// 4. Enable the IRQ for DMA channel 7 global interrupt in NVIC
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}

void enable_DMA_channel7(void)
{
	DMA_Channel_TypeDef* pDMAC7=DMA1_Channel7;

	// Enable the stream
	pDMAC7->CCR |= 1;
}

void HT_Complete_callback(void)
{

}

void FT_Complete_callback(void)
{
	USART_TypeDef* pUSART2=USART2;
	DMA_Channel_TypeDef* pDMAC7=DMA1_Channel7;

	// Program number of data items to send
	pDMAC7->CNDTR = sizeof(dataStream);
	// Here we will cancel UART2_TX DMA request to DMA1 controller
	pUSART2->CR3 &= ~(1<<7);
	enable_DMA_channel7();
}

void TE_Error_callback(void)
{
	while(1);
}

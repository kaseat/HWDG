#include "Uart.h"
#include "Clock.h"
#include "iostm8s003f3.h"

ISubscriber* Uart::subscriber = 0;

void Uart::Init(uint32_t baudrate)
{
	// Configure GPIOs
	PD_ODR_bit.ODR5 = 1;
	PD_DDR_bit.DDR5 = 1; 
	PD_DDR_bit.DDR6 = 0;
	PD_CR1_bit.C16 = 0;
	PD_CR2_bit.C26 = 0;

	// Baudrate configuration.
	uint32_t brr = Clock::GetCpuFreq() / baudrate;
	UART1_BRR2 = brr & 0x000F;
	UART1_BRR2 |= brr >> 12;
	UART1_BRR1 = (brr >> 4) & 0x00FF;
        
        // UART configuration.
	UART1_CR1_PIEN = 0;
	UART1_CR1_PCEN = 0;
	UART1_CR1_M = 0;
	UART1_CR1_UART0 = 0;
        
	UART1_CR2_TEN = 1;
	UART1_CR2_REN = 1;
	UART1_CR2_SBK = 0;
	UART1_CR3_STOP = 0;
        
        // Disable all UART interrupts.
	UART1_CR2_TIEN = 0;
	UART1_CR2_TCIEN = 0;
	UART1_CR2_RIEN = 0;
	UART1_CR2_ILIEN = 0;
}

void Uart::SubscribeOnByteRecivied(ISubscriber& sbcr)
{
	subscriber = &sbcr;
	UART1_CR2_RIEN = 1;
}

void Uart::UnsubscribeOnByteRecivied()
{
	UART1_CR2_RIEN = 0;
}

void Uart::SendByte(uint8_t data)
{
	while (!UART1_SR_TXE);
	UART1_DR = data;
}

void Uart::SendData(uint8_t* data, uint8_t len)
{
	while (len--) SendByte(*data++);
}

#pragma vector=UART1_R_RXNE_vector
__interrupt void Uart::OnByteRecivied()
{
	subscriber->Callback(UART1_DR);
}


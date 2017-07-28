#include "ResetTimer.h"
#include "Clock.h"
#include "iostm8s003f3.h"


ISubscriber* ResetTimer::subscriber = 0;

void ResetTimer::Run(uint16_t timebase)
{
	// Set TIM4 overflow interrupt every 1 ms.
	switch (Clock::GetCpuFreq())
	{
	case Freq2Mhz:
		TIM2_PSCR = 0x01;
		break;
	case Freq4Mhz:
		TIM2_PSCR = 0x02;
		break;
	case Freq8Mhz:
		TIM2_PSCR = 0x03;
		break;
	case Freq16Mhz:
		TIM2_PSCR = 0x0F;
		break;
	}
        
        TIM2_ARRH=uint8_t(timebase >> 8);
	TIM2_ARRL=uint8_t(timebase);
	TIM2_CR1_CEN = 1;
}

void ResetTimer::Stop()
{
	TIM2_CR1_CEN = 0;
}

void ResetTimer::SubscribeOnElapse(ISubscriber& sbcr)
{
	subscriber = &sbcr;
        TIM2_IER_UIE = 1;
}

void ResetTimer::UnsubscribeOnElapse()
{
	TIM2_IER_UIE = 0;
}

#pragma vector=TIM2_OVR_UIF_vector
__interrupt void ResetTimer::OnElapse()
{
	TIM2_SR1_UIF = 0;
	subscriber->Callback(0);
}
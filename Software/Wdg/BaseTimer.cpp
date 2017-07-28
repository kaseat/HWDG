#include "BaseTimer.h"
#include "Clock.h"
#include "iostm8s003f3.h"

#define ARR_VAL 0x7C

ISubscriber* Timer::subscriber = 0;

void Timer::Run()
{
	// Set TIM4 overflow interrupt every 1 ms.
	switch (Clock::GetCpuFreq())
	{
	case Freq2Mhz:
		TIM4_PSCR = 0x04;
		break;
	case Freq4Mhz:
		TIM4_PSCR = 0x05;
		break;
	case Freq8Mhz:
		TIM4_PSCR = 0x06;
		break;
	case Freq16Mhz:
		TIM4_PSCR = 0x07;
		break;
	}
	TIM4_ARR = ARR_VAL;

	TIM4_IER_UIE = 0;
	TIM4_CR1_CEN = 1;
}

void Timer::Stop()
{
	TIM4_CR1_CEN = 0;
}

void Timer::SubscribeOnElapse(ISubscriber& sbcr)
{
	subscriber = &sbcr;
        TIM4_IER_UIE = 1;
}

void Timer::UnsubscribeOnElapse()
{
	TIM4_IER_UIE = 0;
}

#pragma vector=TIM4_OVR_UIF_vector
__interrupt void Timer::OnElapse()
{
	TIM4_SR_UIF = 0;
	subscriber->Callback(0);
}



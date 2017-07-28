#include "Clock.h"
#include "iostm8s003f3.h"
CpuFreq curentFreq = Freq2Mhz;

CpuFreq Clock::GetCpuFreq()
{
  return curentFreq;
}

void Clock::SetCpuFreq(CpuFreq freq)
{
	curentFreq = freq;
	switch (freq)
	{
	case Freq2Mhz:
		CLK_CKDIVR = 3;
		break;
	case Freq4Mhz:
		CLK_CKDIVR = 2;
		break;
	case Freq8Mhz:
		CLK_CKDIVR = 1;
		break;
	case Freq16Mhz:
		CLK_CKDIVR = 0;
		break;
	}
}
#include "GpioRebooter.h"
#include "ResetTimer.h"
#include "iostm8s003f3.h"

#define FR_TIMEOUT 0x10U
#define RST_TIM 0x20U
#define HR_LO_TIM 0xC00U
#define HR_HI_TIM 0x600U

GpioRebooter::GpioRebooter()
{
	PD_ODR_bit.ODR4 = 1;
	PD_DDR_bit.DDR4 = 1;
	PA_ODR_bit.ODR1 = 1;
	PA_DDR_bit.DDR1 = 1;
	firstRun = true;
	srMarker = false;
	hrMarker = false;
	ResetTimer::SubscribeOnElapse(*this);
	ResetTimer::Run(FR_TIMEOUT);
}

GpioRebooter::~GpioRebooter()
{
	ResetTimer::UnsubscribeOnElapse();
}

void GpioRebooter::HardReset()
{
	PD_ODR_bit.ODR4 = 0;
	srMarker = false;
	hrMarker = true;
	ResetTimer::Run(HR_LO_TIM);
}

void GpioRebooter::SoftReset()
{
	PA_ODR_bit.ODR1 = 0;
	srMarker = false;
	hrMarker = false;
	ResetTimer::Run(RST_TIM);
}

void GpioRebooter::Callback(uint8_t data)
{
	// This only need for the first timer run, as timer
	// incorrect behaviour during first run is observed.
	if (firstRun)
	{
		firstRun = false;
		srMarker = false;
		hrMarker = false;
		ResetTimer::Stop();
		return;
	}

	ResetTimer::Stop();

	// usually we reach this point after pin been pulled down
	// so we have to bring pin in floating input mode
	PD_ODR_bit.ODR4 = 1;
	PA_ODR_bit.ODR1 = 1;

	// finally make soft reset on PWR pin
	if (srMarker)
	{
		PD_ODR_bit.ODR4 = 0;
		ResetTimer::Run(RST_TIM);
		srMarker = false;
		hrMarker = false;
	}

	// if we have hard reset we should wait couple of
	// seconds and make soft reset on PWR pin (see above)
	if (hrMarker)
	{
		ResetTimer::Run(HR_HI_TIM);
		hrMarker = false;
		srMarker = true;
	}
}
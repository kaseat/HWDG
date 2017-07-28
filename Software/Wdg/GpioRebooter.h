#pragma once

#include <stdint.h>
#include "IRebooter.h"
#include "ISubscriber.h"

class GpioRebooter : ISubscriber, public IRebooter
{
public:
	GpioRebooter();
	virtual ~GpioRebooter();
	void HardReset();
	void SoftReset();

private:
	bool firstRun;
	bool srMarker;
	bool hrMarker;
	void Callback(uint8_t data);
};
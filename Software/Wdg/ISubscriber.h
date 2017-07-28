#pragma once

class ISubscriber
{
public:
	virtual void Callback(uint8_t data) = 0;

protected:
	~ISubscriber() {}
};

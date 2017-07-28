#pragma once

#include <stdint.h>
#include "ISubscriber.h"

class Timer
{
public:
	/**
	* \brief Run timer as 1 ms timebase.
	*/
	static void Run();
        
	/**
	* \brief Stop timer.
	*/
	static void Stop();
        
	/**
	* \brief Add handler on timer elapse.
	* \param sbcr Subscriber.
	*/
	static void SubscribeOnElapse(ISubscriber& sbcr);
        
	/**
	* \brief Remove handler on timer elapse.
	*/
	static void UnsubscribeOnElapse();
        
	/**
	 * \brief Code we execute on timer elapse;
	 */
        
private:
	static ISubscriber* subscriber;
	__interrupt static void OnElapse();
};
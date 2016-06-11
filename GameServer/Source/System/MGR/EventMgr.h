#pragma once
#ifndef __EFNET_MGR_H__
#define __EFNET_MGR_H__

#include "../../Common.h"

struct EventType
{
	unsigned int KeyID;
	unsigned int WakeUpTime;
	int EventID;

	EventType()
	{
		KeyID = 0;
		WakeUpTime = 0;
		EventID = 0;
	}
};

class EventComp
{
public:
	bool operator() (const EventType LHS, const EventType RHS ) const
	{
		return (LHS.WakeUpTime > RHS.WakeUpTime);
	}
private:
};

class TimerEventMgr
{
public:
	void PushEvent( unsigned int keyID, unsigned int WakeTime, int EventID );

private:
	std::priority_queue<EventType, std::vector<EventType>, EventComp> mTimerQueue;
	std::mutex mTimerlock;
};

#endif 
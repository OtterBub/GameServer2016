#include "EventMgr.h"
#include "../Connect.h"
#include "../../Global.h"


void TimerEventMgr::PushEvent(unsigned int keyID, unsigned int WakeTime, int EventID)
{
	mTimerlock.lock();
	mTimerQueue.push(EventType{ keyID, WakeTime, EventID });
	mTimerlock.unlock();
}

std::priority_queue<EventType, std::vector<EventType>, EventComp>& TimerEventMgr::GetQueue()
{
	return mTimerQueue;
}

void TimerEventMgr::TimerThread()
{
	while (true)
	{
		_sleep(1);
		TIMERMGR.mTimerlock.lock();
		while (false == TIMERQUEUE.empty())
		{
			if (TIMERQUEUE.top().WakeUpTime > GetTickCount()) break;
			EventType ev = TIMERQUEUE.top();
			TIMERQUEUE.pop();
			TIMERMGR.mTimerlock.unlock();
			
			OverlapEx *over = new OverlapEx;
			over->operation = ev.EventID;
			PostQueuedCompletionStatus(GLOBAL.mhIocp, 1,
				ev.KeyID,
				&(over->OriginalOverlap));
			TIMERMGR.mTimerlock.lock();
		}
		TIMERMGR.mTimerlock.unlock();
	}
}
#include "event_queue.h"

#include "event.h"
#include "types.h"

#include <algorithm>

namespace sim
{
	bool EventQueue::empty() const noexcept
	{
		return eventQueue.empty();
	}

	Event EventQueue::popEvent()
	{
		Event e = eventQueue.top();
		eventQueue.pop();
		return e;
	}

	void EventQueue::pushEvent(Event e)
	{
		eventQueue.push(e);
	}

	void EventQueue::pushEvents(Events es)
	{
		std::for_each(es.begin(), es.end(), [this](Event e){ pushEvent(e); });
	}

	UnitDuration EventQueue::time() const
	{
		return eventQueue.top().time;
	}
}

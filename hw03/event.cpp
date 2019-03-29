#include "event.h"

#include "types.h"

namespace sim
{
	bool operator<(Event lhs, Event rhs)
	{
		if(lhs.time == rhs.time)
		{
			return lhs.type < rhs.type;
		}
		return lhs.time < rhs.time;
	}

	bool operator>(Event lhs, Event rhs)
	{
		if(lhs.time == rhs.time)
		{
			return lhs.type > rhs.type;
		}
		return lhs.time > rhs.time;
	}
}

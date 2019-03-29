#pragma once

#include "types.h"

namespace sim
{
	struct Event
	{
		enum TYPE : short
		{
			PROC_ARRIVAL = 0,
			CPU_BRUST_COMPLETION,
			IO_COMPLETION
		};

		TYPE type;
		UnitDuration time;

		union
		{
			procid_t pid;
		};
	};

	bool operator<(Event lhs, Event rhs);
	bool operator>(Event lhs, Event rhs);
}

#pragma once

#include <forward_list>
#include <chrono>
#include <utility>

namespace sim
{
	struct Process;
	struct Event;

	using UnitDuration = std::chrono::milliseconds;
	using procid_t = unsigned int;
	using TimeProcPair = std::pair<UnitDuration, Process*>;

	using Events = std::forward_list<Event>;
}

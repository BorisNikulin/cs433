#pragma once

#include "types.h"

#include <unordered_map>

namespace sim
{

	struct ProcessStats
	{
		UnitDuration arrivalTime;
		UnitDuration finishTime;
		UnitDuration cpuTime;
		UnitDuration ioTime;
		UnitDuration waitTime;
	};

	class  OSStats
	{
		private:
			std::unordered_map<procid_t, ProcessStats> procStats;

		public:
			
			/// Gets ProcessStats by id and creates one if it does not exist.
			ProcessStats& processStatsById(procid_t pid);

			
			// It's 2pm on the day of due date so im going to skip proper encapsulation
			// todo: add iterators to OSStats (expose the underlying map's)
			std::unordered_map<procid_t, ProcessStats>& processStats()
			{ return procStats; }
	};
}

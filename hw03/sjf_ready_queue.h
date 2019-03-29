#pragma once

#include "types.h"
#include "process.h"

#include <queue>
#include <vector>
#include <functional>

namespace sim
{
	class SJFReadyQueue
	{
		private:
			/// Returns which is greater so the pqueue becomes a min pqueue
			// cant use auto here??? c++11 plz
			std::function<bool(TimeProcPair, TimeProcPair)> sjfThenTimeComp = [](TimeProcPair p1, TimeProcPair p2) -> bool
			{
				if(p1.second->nextCpuBurst == p2.second->nextCpuBurst)
				{
					return p1.first > p2.first;
				}
				return p1.second->nextCpuBurst > p2.second->nextCpuBurst;
			};

			std::priority_queue<
				TimeProcPair,
				std::vector<TimeProcPair>,
				decltype(sjfThenTimeComp)> readyQueue;

		public:
			bool empty() const noexcept
			{ return readyQueue.empty(); }

			void pushProcess(UnitDuration startTime, Process* proc);
			TimeProcPair popProccess();
	};
}

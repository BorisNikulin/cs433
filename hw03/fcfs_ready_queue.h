#pragma once

#include "types.h"

#include <queue>

namespace sim
{
	class FCFSReadyQueue
	{
		private:
			std::queue<TimeProcPair> readyQueue;

		public:
			bool empty() const noexcept
			{ return readyQueue.empty(); }

			void pushProcess(UnitDuration startTime, Process* proc);
			TimeProcPair popProccess();
	};
}

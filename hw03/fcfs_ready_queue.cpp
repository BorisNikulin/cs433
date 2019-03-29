#include "fcfs_ready_queue.h"

#include "types.h"

#include <queue>
#include <utility>

namespace sim
{
	void FCFSReadyQueue::pushProcess(UnitDuration startTime, Process* proc)
	{
		readyQueue.push({startTime, proc});
	}

	TimeProcPair FCFSReadyQueue::popProccess()
	{
		TimeProcPair timeProc = readyQueue.front();
		readyQueue.pop();
		return timeProc;
	}
}

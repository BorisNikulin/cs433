#include "sjf_ready_queue.h"

#include "types.h"
#include "process.h"

#include <utility>

namespace sim
{
	void SJFReadyQueue::pushProcess(UnitDuration startTime, Process* proc)
	{
		readyQueue.push({startTime, proc});
	}

	TimeProcPair SJFReadyQueue::popProccess()
	{
		TimeProcPair timeProc = readyQueue.top();
		readyQueue.pop();
		return timeProc;
	}
}

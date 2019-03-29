#include "io_queue.h"

#include "types.h"
#include "process.h"

#include <utility>

namespace sim
{
	void IOQueue::pushProcess(UnitDuration startTime, Process* proc)
	{
		ioBlockedProcesses.emplace(proc->pid, std::make_pair(startTime, proc));
	}

	TimeProcPair IOQueue::popProccessById(procid_t pid)
	{
		auto it = ioBlockedProcesses.find(pid);
		auto pidAndTimeProc = *it;
		TimeProcPair ret = pidAndTimeProc.second;
		ioBlockedProcesses.erase(it);
		return ret;
	}
}

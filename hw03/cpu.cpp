#include "cpu.h"

#include "types.h"
#include "process.h"

#include <utility>

namespace sim
{
	bool Cpu::hasAvailableCpu() const
	{
		return runningProcess.size() < MAX_LOGICAL_CPUS;
	}

	void Cpu::runProcess(UnitDuration startTime, Process* proc)
	{
		runningProcess.emplace(proc->pid, std::make_pair(startTime, proc));
	}

	TimeProcPair Cpu::removeProcessById(procid_t pid)
	{
		auto it = runningProcess.find(pid);
		auto pidAndTimeProc = *it;
		TimeProcPair ret = pidAndTimeProc.second;
		runningProcess.erase(it);
		return ret;
	}
}

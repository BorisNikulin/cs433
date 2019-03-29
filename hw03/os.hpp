#pragma once

#include "types.h"
#include "event.h"
#include "sim_random.hpp"
#include "process.h"
#include "cpu.h"
#include "io_queue.h"
#include "stats.h"

#include <unordered_map>
#include <memory>
#include <stdexcept>

namespace sim
{
	/// Simulated an OS with a given rng generator type and run queue scheduler type.
	template<typename RngGen, typename ProcessScheduler>
	class OS
	{
		private:
			procid_t nextPid;

			/// Owns all Process pointers.
			std::unordered_map<procid_t, std::unique_ptr<Process>> procTable;

			Random<RngGen> random;
			Cpu cpu;
			ProcessScheduler readyQueue;
			IOQueue ioQueue;
			OSStats stats;

			/// Checks for an avaiable cpu and tries to run the first Process in the run queue.
			Events tryRunProcess(UnitDuration startTime);

		public:
			OS(RngGen gen) : nextPid(0), random(gen)
			{}

			OSStats& osStats();

			/// Creates designated number of process arrival events.
			Events newProcessEvents(size_t num);

			Events handleEvent(Event e);

	};

	template<typename RngGen, typename ProcessScheduler>
	OSStats& OS<RngGen, ProcessScheduler>::osStats()
	{
		return stats;
	}


	template<typename RngGen, typename ProcessScheduler>
	Events OS<RngGen, ProcessScheduler>::newProcessEvents(size_t num)
	{
		Events processArivalEvents;

		for(size_t i = 0; i < num; ++i)
		{
			Event e;
			e.type = Event::PROC_ARRIVAL;
			e.time = random.newProcessStart();
			processArivalEvents.push_front(e);
		}

		return processArivalEvents;
	}

	template<typename RngGen, typename ProcessScheduler>
	Events OS<RngGen, ProcessScheduler>::tryRunProcess(UnitDuration startTime)
	{
		Events returnEvents;

		if(cpu.hasAvailableCpu() && !readyQueue.empty())
		{
			TimeProcPair waitProc = readyQueue.popProccess();
			// give me c++17 for structured bindings please
			UnitDuration waitStartTime = waitProc.first;
			Process* proc = waitProc.second;

			ProcessStats& procStats = stats.processStatsById(proc->pid);
			procStats.waitTime += startTime - waitStartTime;

			Event e;
			e.type = Event::CPU_BRUST_COMPLETION;
			e.pid = proc->pid;

			if(proc->nextCpuBurst >= proc->remainingCpuDuration)
			{
				e.time = startTime + proc->remainingCpuDuration;

			}
			else
			{
				e.time = startTime + proc->nextCpuBurst;
			}

			cpu.runProcess(startTime, proc);
			returnEvents.push_front(e);
		}

		return returnEvents;
	}

	template<typename RngGen, typename ProcessScheduler>
	Events OS<RngGen, ProcessScheduler>::handleEvent(Event e)
	{
		Events returnEvents;

		switch(e.type)
		{
			case Event::PROC_ARRIVAL:
			{
				auto totalCpuDuration = random.totalCpuDuration();
				auto meanCpuBurst = random.meanCpuBurst();
				std::unique_ptr<Process> proc(new Process{
					nextPid,
					totalCpuDuration,
					totalCpuDuration,
					meanCpuBurst,
					random.cpuBurst(meanCpuBurst),
					UnitDuration::zero()
				});

				ProcessStats& procStats = stats.processStatsById(proc->pid);
				procStats.arrivalTime = e.time;

				++nextPid;
				readyQueue.pushProcess(e.time, proc.get());
				returnEvents.splice_after(returnEvents.before_begin(), tryRunProcess(e.time));
				procTable.insert(std::make_pair(proc->pid, std::move(proc)));
				break;
			}
			case Event::CPU_BRUST_COMPLETION:
			{
				TimeProcPair runTimeAndProc = cpu.removeProcessById(e.pid);
				// give me c++17 for structured bindings please
				UnitDuration runTimeStart = runTimeAndProc.first;
				Process* proc = runTimeAndProc.second;

				ProcessStats& procStats = stats.processStatsById(proc->pid);
				auto cpuTime = e.time - runTimeStart;
				proc->remainingCpuDuration -= cpuTime;
				procStats.cpuTime += cpuTime;

				if(proc->remainingCpuDuration <= UnitDuration(0))
				{
					procStats.finishTime = e.time;

					auto it = procTable.find(proc->pid);
					procTable.erase(it);
				}
				else
				{
					proc->nextCpuBurst = random.cpuBurst(proc->meanCpuBurst);
					proc->nextIOBurst = random.ioBurst();

					ioQueue.pushProcess(e.time, proc);
					Event ioEvent;
					ioEvent.type = Event::IO_COMPLETION;
					ioEvent.time = e.time + proc->nextIOBurst;
					ioEvent.pid = proc->pid;

					returnEvents.push_front(ioEvent);
				}

				returnEvents.splice_after(returnEvents.before_begin(), tryRunProcess(e.time));
				break;
			}
			case Event::IO_COMPLETION:
			{
				TimeProcPair ioTimeAndProc = ioQueue.popProccessById(e.pid);
				// give me c++17 for structured bindings please
				UnitDuration ioTimeStart = ioTimeAndProc.first;
				Process* proc = ioTimeAndProc.second;

				ProcessStats& procStats = stats.processStatsById(proc->pid);
				auto ioTime = e.time - ioTimeStart;
				procStats.ioTime += ioTime;

				readyQueue.pushProcess(e.time, proc);

				returnEvents.splice_after(returnEvents.before_begin(), tryRunProcess(e.time));
				break;
			}
			default:
				throw std::invalid_argument{"Invalid event type."};
		}

		return returnEvents;
	}
}

#pragma once

#include "types.h"
#include "process.h"

#include <unordered_map>
#include <utility>

namespace sim
{
	class Cpu
	{
		private:
			std::unordered_map<procid_t, TimeProcPair> runningProcess;

		public:
			const size_t MAX_LOGICAL_CPUS;

			Cpu(size_t maxLogicalCpus = 1) : MAX_LOGICAL_CPUS(maxLogicalCpus)
			{}

			/// Check for an avaiable CPU.
			bool hasAvailableCpu() const;

			/// Runs a Process on the cpu.
			/**
			 * Does not check if there is a CPU available.
			 * \param startTime the time at which the process started to run.
			 * \param proc the Process to run.
			 */
			void runProcess(UnitDuration startTime, Process* proc);

			/// Stops a process from running.
			/*
			 * Assumes the process exists.
			 *
			 * \returns The the time when the process was first scheduled an a pointer to the process as a pair.
			 */
			TimeProcPair removeProcessById(procid_t pid);
	};
}

#pragma once

#include "types.h"
#include "process.h"

#include <unordered_map>
#include <utility>

namespace sim
{
	/// Simulates an IO queue that is connected to infinite IO devices so there is no waiting for an IO device.
	class IOQueue
	{
		private:
			std::unordered_map<procid_t, TimeProcPair> ioBlockedProcesses; // well go catch it

		public:

			void pushProcess(UnitDuration startTime, Process* proc);

			/// Assumes the process exists.
			TimeProcPair popProccessById(procid_t pid);
	};
}

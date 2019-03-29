#pragma once

#include "types.h"

namespace sim
{
	struct Process
	{
		procid_t pid;

		UnitDuration startTime;

		UnitDuration totalCpuDuration;
		UnitDuration remainingCpuDuration;

		UnitDuration meanCpuBurst;
		UnitDuration nextCpuBurst;

		UnitDuration nextIOBurst;
	};
}

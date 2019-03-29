#include "stats.h"

namespace sim
{
	ProcessStats& OSStats::processStatsById(procid_t pid)
	{
		return procStats[pid];
	}
}

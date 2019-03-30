#pragma once

#include "types.h"

#include <random>
#include <chrono>

namespace sim
{
	namespace time = std::chrono;

	template<typename RngGen>
	class Random
	{
		private:
			RngGen gen;

			using UniformTimeDist = std::uniform_int_distribution<UnitDuration::rep>;

			// distrubtions cant be const because operator() is not const
			// see https://stackoverflow.com/questions/16008271/why-are-c11-random-distributions-mutable
			UniformTimeDist
				newProcessDist{
					UnitDuration::zero().count(),
					time::duration_cast<UnitDuration>(time::minutes(5)).count()};

			UniformTimeDist
				totalCpuDurationDist{
					time::duration_cast<UnitDuration>(time::seconds(1)).count(),
					time::duration_cast<UnitDuration>(time::minutes(1)).count()};

			UniformTimeDist
				meanCpuBurstDist{
					time::duration_cast<UnitDuration>(time::milliseconds(5)).count(),
					time::duration_cast<UnitDuration>(time::milliseconds(100)).count()};

			UniformTimeDist
				ioBurstDist{
					time::duration_cast<UnitDuration>(time::milliseconds(30)).count(),
					time::duration_cast<UnitDuration>(time::milliseconds(100)).count()};
				//nextCpuBurstDist{}

		public:
			Random(RngGen gen) : gen(gen)
			{}

			UnitDuration newProcessStart();
			UnitDuration totalCpuDuration();
			UnitDuration meanCpuBurst();
			UnitDuration cpuBurst(UnitDuration meanCpuBurst);
			UnitDuration ioBurst();
	};

	template<typename RngGen>
	UnitDuration Random<RngGen>::newProcessStart()
	{
		return UnitDuration(newProcessDist(gen));
	}

	template<typename RngGen>
	UnitDuration Random<RngGen>::totalCpuDuration()
	{
		return UnitDuration(totalCpuDurationDist(gen));
	}

	template<typename RngGen>
	UnitDuration Random<RngGen>::meanCpuBurst()
	{
		return UnitDuration(meanCpuBurstDist(gen));
	}

	template<typename RngGen>
	UnitDuration Random<RngGen>::cpuBurst(UnitDuration meanCpuBurst)
	{
		std::poisson_distribution<UnitDuration::rep> cpuBurstDist(meanCpuBurst.count());
		return UnitDuration(cpuBurstDist(gen));
	}

	template<typename RngGen>
	UnitDuration Random<RngGen>::ioBurst()
	{
		return UnitDuration(ioBurstDist(gen));
	}
}

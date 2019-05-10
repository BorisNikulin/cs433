#pragma once

#include "address.h"

#include <random>

namespace memory { namespace replacement
{
	template<typename RngGen>
	class Random
	{
		private:
			RngGen gen;
			std::uniform_int_distribution<page_id_t> randomPageDist;

		public:
			Random(RngGen gen, int numPages) : gen(gen), randomPageDist(0, numPages - 1)
			{}

			void notifyNew(page_id_t pageId);
			void notifyReference(page_id_t pageId);
			page_id_t findVictim();
	};

	template<typename RngGen>
	void Random<RngGen>::notifyNew(page_id_t)
	{
	}

	template<typename RngGen>
	void Random<RngGen>::notifyReference(page_id_t)
	{
	}

	template<typename RngGen>
	page_id_t Random<RngGen>::findVictim()
	{
		return randomPageDist(gen);
	}
} }

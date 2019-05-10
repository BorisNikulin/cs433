#pragma once

#include "address.h"

#include <list>

namespace memory { namespace replacement
{
	class FIFO
	{
		private:
			std::list<page_id_t> queue;

		public:
			void notifyNew(page_id_t pageId);
			void notifyReference(page_id_t pageId);
			page_id_t findVictim();
	};
} }

#pragma once

#include "address.h"

#include <list>
#include <unordered_map>

namespace memory { namespace replacement
{
	class LRU
	{
		private:
			/// Queue where the back is the least recently used
			std::list<page_id_t> queue;
			std::unordered_map<page_id_t, decltype(queue)::iterator> lookupCache;
		
		public:
			void notifyNew(page_id_t pageId);
			void notifyReference(page_id_t pageId);
			page_id_t findVictim();
	};
} }

#include "lru.h"

#include "address.h"

#include <list>
#include <unordered_map>
#include <utility>

namespace memory { namespace replacement
{
	void LRU::notifyNew(page_id_t pageId)
	{
		queue.push_front(pageId);
		lookupCache.insert({pageId, queue.begin()});
	}

	void LRU::notifyReference(page_id_t pageId)
	{
		auto it = lookupCache.find(pageId);
		queue.push_front(pageId);
		queue.erase(it->second);
		it->second = queue.begin();
	}

	page_id_t LRU::findVictim()
	{
		page_id_t victimPage = queue.back();
		lookupCache.erase(victimPage);
		queue.pop_back();
		return victimPage;
	}
} }

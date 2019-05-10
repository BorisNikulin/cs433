#include "fifo.h"

#include "address.h"

#include <list>

namespace memory { namespace replacement
{
	void FIFO::notifyNew(page_id_t pageId)
	{
		queue.push_back(pageId);
	}

	void FIFO::notifyReference(page_id_t)
	{
	}

	page_id_t FIFO::findVictim()
	{
		page_id_t victimPage = queue.front();
		queue.pop_front();
		return victimPage;
	}
} }

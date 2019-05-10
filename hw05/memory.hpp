#pragma once

#include "address.h"

#include <iostream>

#include <algorithm>
#include <chrono>
#include <forward_list>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <utility>

namespace memory
{
	struct Event
	{
		enum Type : short
		{
			REFERENCE, FAULT, REPLACE, FLUSH
		};

		std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
		Type type;
		page_id_t pageId;

		Event(Type type, page_id_t pageId)
			: timestamp(std::chrono::high_resolution_clock::now()),
				type(type),
				pageId(pageId)
		{}
	};

	template<typename Replacer>
	class Memory
	{
		private:
			struct PageEntry
			{
				frame_id_t frame;
				bool valid;
				bool dirty;
			};

			/// page size is 2^PAGE_OFFSET_POWER
			const int PAGE_OFFSET_POWER;
			const int NUM_PAGES;
			/// memory size is 2^MEMORY_POWER
			const int MEMORY_POWER;

			Replacer replacer;
			std::unique_ptr<PageEntry[]> pageTable;
			std::list<page_id_t> freeFrames;

		public:
			template<typename... Args>
			Memory(int pageOffsetPower, int numPages, int memoryPower, Args... args)
				: PAGE_OFFSET_POWER(pageOffsetPower),
					NUM_PAGES(numPages),
					MEMORY_POWER(memoryPower),
					replacer(std::forward<Args>(args)...),
					pageTable(new PageEntry[numPages])
			{
				frame_id_t freeFrame = 0;
				std::generate_n(std::back_inserter(freeFrames),
					1 << (MEMORY_POWER - PAGE_OFFSET_POWER),
					[&]{ return freeFrame++; });

				std::fill_n(pageTable.get(), numPages, PageEntry{0, false, false});
			}

			std::pair<addr_t, std::list<Event>> reference(Address ref);
	};

	std::string eventTypeToString(Event::Type type)
	{
		switch(type)
		{
			case Event::REFERENCE:
				return "reference";
			case Event::FAULT:
				return "fault";
			case Event::REPLACE:
				return "replace";
			case Event::FLUSH:
				return "flush";
			default:
				return "<invalid event type>";
		}
	}

	template<typename Replacer>
	std::pair<addr_t, std::list<Event>> Memory<Replacer>::reference(Address ref)
	{
		std::list<Event> events;

		page_id_t pageId = ref.addr >> PAGE_OFFSET_POWER;
		offset_t pageOffset = ref.addr & ((1 << PAGE_OFFSET_POWER) - 1);

		PageEntry& pageEntry = pageTable[pageId];

		events.push_back(Event(Event::REFERENCE, pageId));

		if(pageEntry.valid)
		{
			replacer.notifyReference(pageId);
		}
		else
		{
			events.push_back(Event(Event::FAULT, pageId));

			if(freeFrames.empty())
			{
				page_id_t victimPage = replacer.findVictim();
				PageEntry& victimEntry = pageTable[victimPage];

				victimEntry.valid = false;

				events.push_back(Event(Event::REPLACE, victimPage));

				if(victimEntry.dirty)
				{
					events.push_back(Event(Event::FLUSH, victimPage));
				}

				pageEntry.frame = victimEntry.frame;
			}
			else
			{
				pageEntry.frame = freeFrames.front();
				freeFrames.pop_front();

				replacer.notifyNew(pageId);
			}

			pageEntry.valid = true;
		}

		pageEntry.dirty |= ref.mode == Mode::WRITE;

		return {(pageEntry.frame << PAGE_OFFSET_POWER) | pageOffset, events};
	}
}

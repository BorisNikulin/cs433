#include "ready_queue.h"

#include "./../data-structs/PriorityQueue.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>

namespace os { namespace proc
{
	ReadyQueue::ReadyQueue() : pq(comp)
	{}

	auto ReadyQueue::insertProc(PCB* pcb) -> void
	{
		pq.push(pcb);
	}

	auto ReadyQueue::removeHighestProc() -> PCB*
	{
		PCB* pcb = pq.top();
		pq.pop();
		return pcb;
	}

	auto ReadyQueue::size() const noexcept -> size_type
	{
		return pq.size();
	}

	auto ReadyQueue::empty() const noexcept -> bool
	{
		return pq.empty();
	}

	auto ReadyQueue::displayQueue() const -> void
	{
		using namespace std;
		for_each(begin(pq.data()), end(pq.data()),
			[](PCB* pcb)
			{
				cout << *pcb << "; ";
			});
	}
} }

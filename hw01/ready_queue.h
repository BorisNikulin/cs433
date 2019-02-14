#pragma once

#include "./../data-structs/PriorityQueue.hpp"
#include "./../data-structs/Vector.hpp"
#include "pcb.h"

#include <functional>

namespace os { namespace proc
{
	class ReadyQueue
	{
		private:
			using pcb_compare = std::function<bool(PCB*, PCB*)>;
			// reverses max heap to a min heap
			// assumes non null pointers
			pcb_compare comp = [](PCB* p1, PCB* p2) -> bool { return *p1 > *p2; };

			using PriorityQueue = typename ::ds::container::PriorityQueue<
				PCB*,
				::ds::container::Vector<PCB*>,
				pcb_compare>;

			PriorityQueue pq;

		public:
			using size_type = PriorityQueue::size_type;

			ReadyQueue();

			auto insertProc(PCB* pcb) -> void;
			auto removeHighestProc() -> PCB*;
			auto size() const noexcept -> size_type;
			auto empty() const noexcept -> bool;
			auto displayQueue() const -> void;
	};
} }

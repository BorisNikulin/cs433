#pragma once

#include "pcb.h"
#include "./../data-structs/UnorderedMap.hpp"

namespace os { namespace proc {

	// owns all pcb pointers
	class PCBTable
	{
		private:
			PCB* head;
			::ds::container::UnorderedMap<pid_t, PCB*> table;

		public:
			PCBTable();
			~PCBTable();

			// once a pcb is in the table
			// it's managed soley by the table
			auto addPCB(PCB* pcb) -> bool;

			// TODO add remove() to UnorderedMap and then to this class

			auto first() const -> PCB*;

			// TODO  making an unchecked const get
			// would require updating UnorderedMap
			//auto operator[](pid_t pid) const -> PCB*;
			auto at(pid_t pid) const -> PCB*;
	};
} }

#include "pcb_table.h"

#include "pcb.h"
#include "./../data-structs/UnorderedMap.hpp"

#include <utility>

namespace os { namespace proc
{
	PCBTable::PCBTable() : head(nullptr), table()
	{}

	PCBTable::~PCBTable()
	{
		if(head)
		{
			PCB* cur = head;
			do
			{
				PCB* next = cur->next;
				delete cur;
				cur = next;
			}
			while(cur != head);
		}
	}

	auto PCBTable::addPCB(PCB* pcb) -> bool
	{
		if(table.insert({pcb->pid, pcb}).second)
		{
			if(!head)
			{
				head = pcb;
				pcb->next = head;
				pcb->prev = head;
			}
			else
			{
				pcb->prev = head->prev;
				head->prev->next = pcb;
				pcb->next = head;
				head->prev = pcb;
			}

			return true;
		}
		return false;
	}

	auto PCBTable::first() const -> PCB*
	{
		return head;
	}

	auto PCBTable::at(pid_t pid) const -> PCB*
	{
		return table.at(pid);
	}
} }

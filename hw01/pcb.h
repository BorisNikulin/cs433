#pragma once

#include "os_types.h"

#include <iostream>
#include <memory>

namespace os { namespace proc
{
	enum class State : short
	{
		NEW, READY, RUNNING, WAITING, TERMINATED
	};

	class PCB
	{
		public:
			PCB* next;
			PCB* prev;

			procid_t pid;
			State state;
			priority_t priority;
			// TODO extra info
	};

	auto stateToString(State s) -> std::string;

} }

namespace std
{
	template<>
	struct hash<os::proc::PCB>;

	template<>
	struct hash<os::proc::PCB*>;

	template<>
	struct hash<std::unique_ptr<os::proc::PCB>>;
}

auto operator==(const os::proc::PCB& lhs, const os::proc::PCB& rhs) -> bool;
auto operator!=(const os::proc::PCB& lhs, const os::proc::PCB& rhs) -> bool;
auto operator<(const os::proc::PCB& lhs, const os::proc::PCB& rhs) -> bool;
auto operator>(const os::proc::PCB& lhs, const os::proc::PCB& rhs) -> bool;

auto operator<<(std::ostream&os, const os::proc::State s) -> std::ostream&;
auto operator<<(std::ostream& os, const os::proc::PCB& pcb) -> std::ostream&;

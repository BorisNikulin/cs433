#include "pcb.h"

#include <iostream>

namespace os { namespace proc
{
	auto stateToString(State s) -> std::string
	{
		switch(s)
		{
			case State::NEW:
				return "new";
			case State::READY:
				return "ready";
			case State::RUNNING:
				return "running";
			case State::WAITING:
				return "waiting";
			case State::TERMINATED:
				return "terminated";
		}
	}
} }

namespace std
{
	template<>
	struct hash<os::proc::PCB>
	{
		// deprecated in C++17
		using argument_type = os::proc::PCB;
		using result_type = std::size_t;

		auto operator()(const argument_type& pcb)-> std::size_t
		{
			return std::hash<os::procid_t>{}(pcb.pid);
		}
	};

	// assumes non null
	template<>
	struct hash<os::proc::PCB*>
	{
		// deprecated in C++17
		using argument_type = os::proc::PCB*;
		using result_type = std::size_t;

		auto operator()(const argument_type& pcb)-> std::size_t
		{
			return std::hash<os::procid_t>{}(pcb->pid);
		}
	};

	// assumes non null
	template<>
	struct hash<std::unique_ptr<os::proc::PCB>>
	{
		// deprecated in C++17
		using argument_type = std::unique_ptr<os::proc::PCB>;
		using result_type = std::size_t;

		auto operator()(const argument_type& pcb)-> std::size_t
		{
			return std::hash<os::procid_t>{}(pcb->pid);
		}
	};
}

auto operator==(const os::proc::PCB& lhs, const os::proc::PCB& rhs) -> bool
{
	return lhs.pid == rhs.pid;
}

auto operator!=(const os::proc::PCB& lhs, const os::proc::PCB& rhs) -> bool
{
	return lhs.pid != rhs.pid;
}

auto operator<(const os::proc::PCB& lhs, const os::proc::PCB& rhs) -> bool
{
	if(lhs.priority == rhs.priority)
	{
		return lhs.pid < rhs.pid;
	}
	return lhs.priority < rhs.priority;
}

auto operator>(const os::proc::PCB& lhs, const os::proc::PCB& rhs) -> bool
{
	if(lhs.priority == rhs.priority)
	{
		return lhs.pid > rhs.pid;
	}
	return lhs.priority > rhs.priority;
}



auto operator<<(std::ostream& os, const os::proc::State s) -> std::ostream&
{
	return os << os::proc::stateToString(s);
}

auto operator<<(std::ostream& os, const os::proc::PCB& pcb) -> std::ostream&
{
	return os << "{PCB: PID = " << pcb.pid
		<< " , Priority = " << pcb.priority
		<< ", State = " << pcb.state
		<< "}";
}

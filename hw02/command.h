#pragma once

#include "program.h"
#include "built_in.h"

#include <deque>
#include <string>

namespace shell
{
	// low level sum type of a built in command and a runnable program
	struct Command
	{

		enum TAG : bool
		{
			BUILT_IN, PROGRAM
		};

		TAG tag;

		union Data
		{
			BuiltIn built_in;
			// program is abstract and needs to be a pointer
			Program* program;//= nullptr;

			Data() {}
			~Data() {}
		} data;

		Command() : tag(BUILT_IN)
		{ data.built_in = BuiltIn{}; }

		~Command();

		Command& operator=(Command&& cmd);
		Command(Command&& cmd)
		{ *this = std::move(cmd); };

		std::deque<std::string> show() const;
	};
}

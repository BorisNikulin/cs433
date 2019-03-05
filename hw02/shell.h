#pragma once

#include "command.h"
#include "built_in.h"
#include "program.h"

#include <string>
#include <deque>
#include <utility>

namespace shell
{
	class Shell
	{
		private:
			std::deque<Command> history;

			std::pair<bool, int> runCommand(const Command& cmd);
			std::pair<bool, int> runBuiltIn(const BuiltIn builtIn);
			std::pair<bool, int> runProgram(const Program* cmd);
			std::pair<bool, int> runHistory(int histIndex);

		public:
			std::pair<bool, int> processCommand(const std::string& cmdStr);
	};
}
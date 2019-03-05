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

			std::pair<bool, int> processBuiltIn(const BUILT_IN builtIn);
			std::pair<bool, int> processProgram(Program* cmd);

		public:
			std::pair<bool, int> processCommand(const std::string& cmdStr);
	};
}

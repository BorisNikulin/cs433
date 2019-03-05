#include "parser.h"

#include "command.h"
#include "program.h"
#include "single_program.hpp"

#include <algorithm>
#include <string>
#include <deque>
#include <memory>

namespace shell
{
	Command parseCommand(const std::string cmdStr)
	{
		std::string copy(cmdStr);
		std::transform(copy.begin(), copy.end(), copy.begin(), tolower);

		if(copy == "exit")
		{
			Command cmd;
			cmd.tag = Command::BUILT_IN;
			cmd.data.built_in = BUILT_IN::EXIT;
			return cmd;
		}
		else if(copy == "history")
		{
			Command cmd;
			cmd.tag = Command::BUILT_IN;
			cmd.data.built_in = BUILT_IN::HISTORY;
			return cmd;
		}


		std::deque<std::string> args;
		std::string::size_type separatorIndex;
		std::string::size_type lastIndex = 0;

		while((separatorIndex = cmdStr.find(" ", lastIndex)) != std::string::npos)
		{
			args.push_back(cmdStr.substr(lastIndex, separatorIndex - lastIndex));
			lastIndex = separatorIndex + 1;
		}

		if(lastIndex < cmdStr.size())
		{
			args.push_back(cmdStr.substr(lastIndex, std::string::npos));
		}

		Command cmd;
		cmd.tag = Command::PROGRAM;
		cmd.data.program = new SingleProgram(args.begin(), args.end(), args.size());

		return cmd;
	}
}

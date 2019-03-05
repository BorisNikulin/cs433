#include "shell.h"

#include "parser.h"
#include "command.h"

#include <iostream>
#include <string>
#include <utility>
#include <cstdlib>

namespace shell
{
	std::pair<bool, int> Shell::processCommand(const std::string& cmdStr)
	{
		return runCommand(parseCommand(cmdStr));
	}

	std::pair<bool, int> Shell::runCommand(Command cmd)
	{
		std::pair<bool, int> ret;

		switch(cmd.tag)
		{
			case Command::BUILT_IN:
				ret = runBuiltIn(cmd.data.built_in);
				break;
			case Command::PROGRAM:
				ret = runProgram(cmd.data.program);
				break;
		}

		std::cout << "command: ";
		for(auto s : cmd.show())
		{
			std::cout << s << " ";
		}
		std::cout << "\n";

		history.push_back(std::move(cmd));
		return ret;
	}

	std::pair<bool, int> Shell::runBuiltIn(const BuiltIn builtIn)
	{
		switch(builtIn.tag)
		{
			case BuiltIn::NO_COMMAND:
				return {false, 0};
			case BuiltIn::EXIT:
				return {true, 0};
			case BuiltIn::HISTORY:
				//return runHistory(builtIn.data.histIndex);
				return {false, 0};
			case BuiltIn::BUILT_IN_ERROR:
				//std::cout << builtIn.toString() << "\n";
				return {false, 1};
			default:
				std::cout << "Unknown BUILT_IN value\n";
				std::exit(1);
		}
	}

	std::pair<bool, int> Shell::runProgram(const Program* cmd)
	{
		cmd->run();
		return {false, 0};
	}

}

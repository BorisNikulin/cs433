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
		Command cmd = parseCommand(cmdStr);
		std::pair<bool, int> ret;

		switch(cmd.tag)
		{
			case Command::BUILT_IN:
				ret = processBuiltIn(cmd.data.built_in);
				break;
			case Command::PROGRAM:
				ret = processProgram(cmd.data.program);
				break;
		}

		std::cout << "\ncommand: ";
		for(auto s : cmd.show())
		{
			std::cout << s << " ";
		}
		std::cout << "\n";

		history.push_back(std::move(cmd));
		return ret;
	}

	std::pair<bool, int> Shell::processBuiltIn(const BUILT_IN builtIn)
	{
		switch(builtIn)
		{
			case BUILT_IN::EXIT:
				return {true, 0};
			default:
				std::cout << "Unknown BUILT_IN value\n";
				std::exit(1);
		}
	}

	std::pair<bool, int> Shell::processProgram(Program* cmd)
	{
		cmd->run();
		return {false, 0};
	}

}

#include "shell.h"

#include "command.h"
#include "parser.hpp"

#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace shell
{
	std::pair<bool, int> Shell::processCommand(const std::string& cmdStr)
	{
		Command cmd = parse::parseCommand(cmdStr);

		if(cmd.tag == Command::BUILT_IN)
		{
			switch(cmd.data.built_in.tag)
			{
				case BuiltIn::NO_COMMAND:
					return {false, 0};
				case BuiltIn::ERROR:
					runBuiltIn(cmd.data.built_in);
					return {false, 1};
				case BuiltIn::HISTORY:
					if(cmd.data.built_in.data.histIndex >= static_cast<int>(history.size()))
					{
						std::cout << "history: no references to current or future commands\n";
						return {false, 1};
					}
					break;
				default:
					break;
			}
		}

		history.push_back(std::move(cmd));
		return runCommand(history.back());
	}

	std::pair<bool, int> Shell::runCommand(const Command& cmd)
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
				return runHistory(builtIn.data.histIndex);
			case BuiltIn::ERROR:
				std::cout << builtIn.toString() << "\n";
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

	std::pair<bool, int> Shell::runHistory(int histIndex)
	{
		if(histIndex >= 0)
		{
			try
			{
				return runCommand(history.at(histIndex));
			}
			catch(const std::out_of_range&)
			{
				std::cout << "history: index (" << histIndex << ") is out of bounds\n";
				return {false, 1};
			}
		}
		else
		{
			int indexWidth = std::ceil(std::log10(history.size()));
			for(size_t i = 0; i < history.size(); ++i)
			{
				std::cout << std::setw(indexWidth) << i
					<< " - ";
				for(const auto& s : history[i].show())
				{
					std::cout << s << " ";
				}
				std::cout << "\n";
			}

			return {false, 0};
		}
	}
}

#include "shell.h"

#include "parser.h"
#include "command.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include <cstdlib>
#include <stdexcept>
#include <cmath>

namespace shell
{
	std::pair<bool, int> Shell::processCommand(const std::string& cmdStr)
	{
		Command cmd = parseCommand(cmdStr);

		// allow only stricly monotonically decreasing history references to prevent loops and thus segfaults
		if(cmd.tag == Command::BUILT_IN
			&& cmd.data.built_in.tag == BuiltIn::HISTORY
			&& cmd.data.built_in.data.histIndex >= static_cast<int>(history.size()))
		{
			std::cout << "history: no references to current or future commands\n";
			return {false, 1};
		}
		else if(cmd.tag == Command::BUILT_IN
			&& cmd.data.built_in.tag == BuiltIn::NO_COMMAND)
		{
			return {false, 0};
		}
		else
		{
			history.push_back(std::move(cmd));
			return runCommand(history.back());
		}
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

		std::cout << "command: ";
		for(auto s : cmd.show())
		{
			std::cout << s << " ";
		}
		std::cout << "\n";

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

	std::pair<bool, int> Shell::runHistory(int histIndex)
	{
		if(histIndex >= 0)
		{
			try
			{
				const auto& cmd = history.at(histIndex);

				// prevent self loop seg fault
				if(cmd.tag == Command::BUILT_IN
					&& cmd.data.built_in.tag == BuiltIn::HISTORY
					&& cmd.data.built_in.data.histIndex == histIndex)
				{
					//std::cout << "lol\n";
					return {false, 1};
				}

				return runCommand(cmd);
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

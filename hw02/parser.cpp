#include "parser.h"

#include "command.h"
#include "program.h"
#include "single_program.hpp"

#include <algorithm>
#include <string>
#include <deque>
#include <memory>

#include <iostream>

namespace shell { namespace parse
{
	Command parseCommand(const std::string cmdStr)
	{
		// trim whitepace
		std::string trimmedCmdStr = cmdStr;
		const auto whiteSpace = " \t\n";
		std::string::size_type whiteSpaceIndex;
		auto createNoCommand = []() -> Command
		{
			Command cmd;
			cmd.tag = Command::BUILT_IN;
			cmd.data.built_in = BuiltIn(BuiltIn::NO_COMMAND);
			return cmd;
		};

		if((whiteSpaceIndex = trimmedCmdStr.find_first_not_of(whiteSpace)) != std::string::npos)
		{
			trimmedCmdStr.erase(0, whiteSpaceIndex);
		}
		else
		{
			return createNoCommand();
		}

		if((whiteSpaceIndex = trimmedCmdStr.find_last_not_of(whiteSpace)) != std::string::npos)
		{
			trimmedCmdStr.erase(whiteSpaceIndex + 1);
		}
		else
		{
			return createNoCommand();
		}

		// built in commands parsing
		std::string copy(trimmedCmdStr);
		std::transform(copy.begin(), copy.end(), copy.begin(), tolower);

		if(copy == "exit")
		{
			Command cmd;
			cmd.tag = Command::BUILT_IN;
			cmd.data.built_in = BuiltIn(BuiltIn::EXIT);
			return cmd;
		}
		else if(copy == "history" || copy == "!!")
		{
			Command cmd;
			cmd.tag = Command::BUILT_IN;
			BuiltIn builtIn(BuiltIn::HISTORY);
			builtIn.data.histIndex = -1;
			cmd.data.built_in = builtIn;
			return cmd;
		}

		// program parsing and history with arg parsing
		std::deque<std::string> args;
		std::string::size_type separatorIndex;
		std::string::size_type lastIndex = 0;
		Command cmd;
		bool runInBackground = false;

		while((separatorIndex = trimmedCmdStr.find_first_of(whiteSpace, lastIndex)) != std::string::npos)
		{
			args.push_back(trimmedCmdStr.substr(lastIndex, separatorIndex - lastIndex));
			lastIndex = trimmedCmdStr.find_first_not_of(whiteSpace, separatorIndex);
		}

		if(lastIndex < trimmedCmdStr.size())
		{
			args.push_back(trimmedCmdStr.substr(lastIndex, std::string::npos));
		}

		// parse built in history with arguemnt
		if(args.size() == 2 && (args[0] == "history" || args[0] == "!!"))
		{
			BuiltIn hist(BuiltIn::HISTORY);
			try
			{
				hist.data.histIndex = std::stoi(args[1]);
			}
			catch(...)
			{
				BuiltIn error(BuiltIn::BUILT_IN_ERROR);
				error.data.what = "history: invalid history index";

				cmd.tag = Command::BUILT_IN;
				cmd.data.built_in = error;
				return cmd;
			}

			cmd.tag = Command::BUILT_IN;
			cmd.data.built_in = hist;
			return cmd;
		}
		else if(args.size() > 0 && args.back() == "&")
		{
			runInBackground = true;
			args.pop_back();
		}

		cmd.tag = Command::PROGRAM;
		cmd.data.program = new SingleProgram(args.begin(), args.end(), args.size());
		cmd.data.program->runInBackground = runInBackground;

		return cmd;
	}
} }

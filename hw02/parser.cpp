#include "parser.hpp"

#include "command.h"
#include "program.h"
#include "single_program.hpp"

#include <algorithm>
#include <string>
#include <deque>

#include <iostream>

namespace shell { namespace parse
{
	Command parseCommand(const std::string& cmdStr)
	{
		Command cmd;
		std::string trimmedCmdStr = cmdStr;
		trim(trimmedCmdStr);

		if(trimmedCmdStr.empty())
		{
			cmd.tag = Command::BUILT_IN;
			cmd.data.built_in = BuiltIn(BuiltIn::NO_COMMAND);
			return cmd;
		};

		if(!(cmd = parseNoArgBuiltIn(trimmedCmdStr)).isNoCommand())
		{
			return cmd;
		}

		return parseCommandWithArgs(trimmedCmdStr);
	}

	void trim(std::string& str)
	{
		std::string::size_type whiteSpaceIndex;

		if((whiteSpaceIndex = str.find_first_not_of(whiteSpace)) != std::string::npos)
		{
			str.erase(0, whiteSpaceIndex);
		}
		else
		{
			str.clear();
			return;
		}

		if((whiteSpaceIndex = str.find_last_not_of(whiteSpace)) != std::string::npos)
		{
			str.erase(whiteSpaceIndex + 1);
		}
		else
		{
			str.clear();
			return;
		}
	}

	Command parseNoArgBuiltIn(const std::string& trimmedCmdStr)
	{
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

		return Command{};
	}

	Command parseCommandWithArgs(const std::string& trimmedCmdStr)
	{
		Command cmd;
		std::deque<std::string> args;
		std::string::size_type separatorIndex;
		std::string::size_type lastIndex = 0;

		while((separatorIndex = trimmedCmdStr.find_first_of(whiteSpace, lastIndex)) != std::string::npos)
		{
			args.push_back(trimmedCmdStr.substr(lastIndex, separatorIndex - lastIndex));
			lastIndex = trimmedCmdStr.find_first_not_of(whiteSpace, separatorIndex);
		}

		if(lastIndex < trimmedCmdStr.size())
		{
			args.push_back(trimmedCmdStr.substr(lastIndex, std::string::npos));
		}


		if(!(cmd = parseBuiltinWithArgs(args)).isNoCommand())
		{
			return cmd;
		}


		return parseProgram(args.begin(), args.end());
	}

	Command parseBuiltinWithArgs(const std::deque<std::string>& args)
	{
		Command cmd;

		if(args.size() == 2 && (args[0] == "history" || args[0] == "!!"))
		{
			BuiltIn hist(BuiltIn::HISTORY);
			try
			{
				hist.data.histIndex = std::stoi(args[1]);
			}
			catch(...)
			{
				return makeError("history: invalid history index");
			}

			cmd.tag = Command::BUILT_IN;
			cmd.data.built_in = hist;
			return cmd;
		}

		return Command{};
	}
} }

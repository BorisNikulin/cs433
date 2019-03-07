#pragma once

#include "command.h"
#include "built_in.h"
#include "single_program.hpp"
#include "util.hpp"

#include <string>
#include <algorithm>

#include <iostream>

namespace shell
{
/**
 * Namespace for all Command parsing.
 *
 * \namespace shell::parse
 */
namespace parse
{
	const std::string whiteSpace = " \t\n";

	/// Parses a single line of Shell input.
	/**
	 * Parses a line of input that must not have trailing new lines into a Command.
	 *
	 * \param cmdStr single line of input.
	 * \return A command suitable to be run.
	 */
	Command parseCommand(const std::string& cmdStr);

	/// Trims whitepsace from the left and from the right.
	void trim(std::string& str);

	/// Parses any built in command that does not take arguments.
	/**
	 * \param trimmedCmdStr single line of input trimmed of white space on both sides.
	 * \returns A command that may be the built in no command if there is nothing to parse.
	 */
	Command parseNoArgBuiltIn(const std::string& trimmedCmdStr);

	/// Parses any command that takes arguments.
	/**
	 * \param trimmedCmdStr single line of input trimmed of white space on both sides.
	 * \returns A command suitable to be run.
	 */
	Command parseCommandWithArgs(const std::string& trimmedCmdStr);

	/// Parses any built in command that takes arguments.
	/**
	 * \param args list of space separated string tokens.
	 * \returns A command that may be the built in no command if there is nothing to parse.
	 */
	Command parseBuiltinWithArgs(const std::deque<std::string>& args);

	/// Parses any single program.
	/**
	 * \param start starting bidirectional iterator to a list of space separated string tokens
	 * \param end ending bidirectional iterator to a list of space separated string tokens
	 * \returns A command suitable to be run.
	 */
	template<typename BiDirIt>
	Command parseProgram(IterWithValueType<BiDirIt, std::string> start, BiDirIt end)
	{
		Command cmd;
		bool runInBackground = false;

		if(start != end && *(end - 1) == "&")
		{
			runInBackground = true;
			--end;
		}

		cmd.tag = Command::PROGRAM;
		cmd.data.program = new SingleProgram(start, end, std::distance(start, end));
		cmd.data.program->runInBackground = runInBackground;

		return cmd;
	}
}
}

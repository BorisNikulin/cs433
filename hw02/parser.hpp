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
	 * \param trimedCmdStr single line of input trimmed of whitespace on both sides.
	 * \returns A command that may be the built in no command if there is nothing to parse.
	 */
	Command parseNoArgBuiltIn(const std::string& trimmedCmdStr);

	/// Parses any command that takes arguments.
	/**
	 * \param trimedCmdStr single line of input trimmed of whitespace on both sides.
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

		auto findRedirectChar = [](const std::string& s) -> int
		{
			if(s.size() > 0)
			{
				if(s.front() == '<' || s.front() == '>')
				{
					return 0;
				}
				else if(s.size() > 1)
				{
					return s[1] == '<' || s[1] == '>' ? 1 : -1;
				}
			}

			return -1;
		};

		int lastRedirectCharIndex;
		auto isRedirectToken = [&](const std::string& s)
		{
			lastRedirectCharIndex = findRedirectChar(s);
			return lastRedirectCharIndex != -1;
		};


		auto firstRedirect = std::find_if(start, end, isRedirectToken);

		int streamToRedirect;
		if(lastRedirectCharIndex != -1)
		{
			std::string file;

			if(lastRedirectCharIndex == 0)
			{
				if(firstRedirect->front() == '<')
				{
					streamToRedirect = STDIN_FILENO;
				}
				else
				{
					streamToRedirect = STDOUT_FILENO;
				}

				auto next = firstRedirect;
				++next;

				if(next != end)
				{

				}
				else
				{
					return makeError("redirection must name a file");
				}

			}
			else if(lastRedirectCharIndex > 0)
			{
				if((*firstRedirect)[lastRedirectCharIndex] == '>')
				{
					streamToRedirect = firstRedirect->front() - '0';
				}
				else
				{
					return makeError("redirection of input only allowed for standard input");
				}
			}
		}

		std::cout << "redir: " << streamToRedirect << "\n";

		cmd.tag = Command::PROGRAM;
		// assumes redirection is at the end or just before &
		cmd.data.program = new SingleProgram(start, firstRedirect, std::distance(start, firstRedirect));
		cmd.data.program->runInBackground = runInBackground;

		return cmd;
	}
}
}

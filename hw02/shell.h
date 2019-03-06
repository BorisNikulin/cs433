#pragma once

#include "command.h"
#include "built_in.h"
#include "program.h"

#include <string>
#include <deque>
#include <utility>

/**
 * Namespace for all shell related code.
 *
 * \namespace shell
 */
namespace shell
{

	/// Class for consuming shell commands and interpreting them.
	/**
	 * The Shell class handles
	 * - parsing
	 * - running Command%s
	 * - saving previous Command%s
	 *
	 * Shell does not handle input.
	 * The class operates on passed in lines.
	 *
	 * \sa Command
	 */
	class Shell
	{
		private:
			/// Owns all Command s in the shell as well as providing history.
			std::deque<Command> history;


			/// Runs any command. Main entry point for Command%s.
			std::pair<bool, int> runCommand(const Command& cmd);
			std::pair<bool, int> runBuiltIn(const BuiltIn builtIn);
			std::pair<bool, int> runProgram(const Program* cmd);

			/// Run the history built in command
			/**
			 * The history command either displays the entire command history since shell launch,
			 * or runs a previous command.
			 *
			 * runHistory can handle any index value.
			 *
			 * \param histIndex negative to show history, positive to run a previous command.
			 */
			std::pair<bool, int> runHistory(int histIndex);

		public:

			/// Interprets one line of input.
			/**
			 * \param cmdStr one line of input with out a trailing new line
			 * \return A pair where the first is a boolean for requesting Shell exit
			 * and the second being program return codes in the `POSIX` style.
			 */
			std::pair<bool, int> processCommand(const std::string& cmdStr);
	};
}

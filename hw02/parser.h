#pragma once

#include "command.h"

#include <string>

namespace shell
{
/**
 * Namespace for all Command parsing.
 *
 * \namespace shell::parse
 */
namespace parse
{
	/// Parses a single line of Shell input
	/**
	 * Parses a line of input that must not have trailing new lines into a Command.
	 *
	 * \param cmdStr single line of input
	 * \return A command suitible to be run.
	 */
	Command parseCommand(const std::string cmdStr);
}
}

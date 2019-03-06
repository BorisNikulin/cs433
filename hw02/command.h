#pragma once

#include "program.h"
#include "built_in.h"

#include <deque>
#include <string>

namespace shell
{
	/// Sum type for built in commands and programs.
	/**
	 * tag determines the type of the sum type or tagged union.
	 *
	 * Care must be taken when accessing Command::data to ensure
	 * the proper type of variable is being accessed,
	 *
	 * Command is not copy constructable or assignable.
	 * There must be no aliases.
	 *
	 * \sa BuiltIn, Program
	 */
	struct Command
	{

		enum TAG : bool
		{
			BUILT_IN, PROGRAM
		};

		TAG tag;

		union Data
		{
			BuiltIn built_in;
			// program is abstract and needs to be a pointer
			Program* program;//= nullptr;

			Data() {}
			~Data() {}
		} data;

		Command() : tag(BUILT_IN)
		{ data.built_in = BuiltIn{}; }

		~Command();

		Command& operator=(Command&& cmd);
		Command(Command&& cmd)
		{ *this = std::move(cmd); };

		/// Obtains a command in the form of a list of space separated elements for displaying.
		std::deque<std::string> show() const;
	};
}

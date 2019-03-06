#pragma once

#include <string>
#include <cstring>

namespace shell
{
	/// Sum type for all the built in Command%s
	struct BuiltIn
	{
		enum TAG : short
		{
			NO_COMMAND, EXIT, HISTORY, BUILT_IN_ERROR
		};

		TAG tag;

		union Data
		{
			/// Index for the history command.
			/**
			 * \sa Shell::runHistory()
			 */
			int histIndex;

			/// Erorr message for BUILT_IN_ERROR
			std::string what;

			Data() {}
			~Data() {}

		} data;

		BuiltIn() : tag(NO_COMMAND) {}

		BuiltIn(TAG tag) : tag(tag) {}

		~BuiltIn()
		{ if(tag == BUILT_IN_ERROR) data.what.~basic_string(); }

		BuiltIn& operator=(const BuiltIn& builtIn);
		BuiltIn(const BuiltIn& builtIn)
		{ *this = builtIn; }

		/// Returns a string representation of the built in command.
		std::string toString() const;
	};
}

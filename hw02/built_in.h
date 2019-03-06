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
			NO_COMMAND, EXIT, HISTORY, ERROR
		};

		TAG tag;

		union Data
		{
			/// Index for the history command.
			/**
			 * \sa Shell::runHistory()
			 */
			int histIndex;

			/// Error message for ERROR
			std::string what;

			Data() {}
			~Data() {}

		} data;

		BuiltIn() : tag(NO_COMMAND) {}

		BuiltIn(TAG tag) : tag(tag)
		{ if(tag == ERROR) new(&data.what) std::string; }

		~BuiltIn()
		{ if(tag == ERROR) data.what.~basic_string(); }

		BuiltIn& operator=(const BuiltIn& builtIn);
		BuiltIn(const BuiltIn& builtIn)
		{ *this = builtIn; }

		/// Returns a string representation of the built in command.
		std::string toString() const;
	};
}

#pragma once

#include <string>
#include <cstring>

namespace shell
{
	struct BuiltIn
	{
		enum TAG : short
		{
			NO_COMMAND, EXIT, HISTORY, BUILT_IN_ERROR
		};

		TAG tag;

		union Data
		{
			int histIndex;
			std::string what;

			Data() {}
			~Data() {}

		} data;

		BuiltIn() : tag(NO_COMMAND) {}

		// does not stop segfaults unless you use placment new to set what /shrug
		BuiltIn(TAG tag) : tag(tag) {}
		//{ if(tag == BUILT_IN_ERROR) new(&data.what) std::string{}; }

		~BuiltIn()
		{ if(tag == BUILT_IN_ERROR) data.what.~basic_string(); }

		BuiltIn& operator=(const BuiltIn& builtIn);
		BuiltIn(const BuiltIn& builtIn)
		{ *this = builtIn; }

		std::string toString() const;
	};
}

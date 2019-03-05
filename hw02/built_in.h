#pragma once

#include <string>

namespace shell
{
	enum class BUILT_IN : short
	{
		EXIT, HISTORY
	};

	std::string builtInToString(const BUILT_IN builtIn);
}

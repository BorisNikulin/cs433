#include "built_in.h"

#include <string>

namespace shell
{
	std::string builtInToString(const BUILT_IN builtIn)
	{
		switch(builtIn)
		{
			case BUILT_IN::EXIT:
				return "exit";
			case BUILT_IN::HISTORY:
				return "history";
			default:
				return "Unkown BUILT_IN";
		}			
	}
}

#include "built_in.h"

#include <string>
#include <sstream>

#include <iostream>

namespace shell
{
	BuiltIn& BuiltIn::operator=(const BuiltIn& builtIn)
	{
		tag = builtIn.tag;
		switch(tag)
		{
			case HISTORY:
				data.histIndex = builtIn.data.histIndex;
				break;
			case ERROR:
				//std::couO << "copy: " << builtIn.data.what << "\n";
				new(&data.what) std::string(builtIn.data.what);
				break;
			default:
				break;
		}
		return *this;
	}


	std::string BuiltIn::toString() const
	{
		switch(tag)
		{
			case BuiltIn::NO_COMMAND:
				return "empty command";
			case BuiltIn::EXIT:
				return "exit";
			case BuiltIn::HISTORY:
			{
				std::ostringstream ss;
				ss << "history";
				if(data.histIndex >= 0)
				{
					ss << " " << data.histIndex;
				}
				return ss.str();
			}
			case BuiltIn::ERROR:
				return data.what;
			default:
				return "Unknown BUILT_IN";
		}
	}
}

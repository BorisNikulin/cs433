#include "command.h"

#include <deque>
#include <string>

namespace shell
{
		Command& Command::operator=(Command&& cmd)
		{
			tag = cmd.tag;
			data = cmd.data;

			cmd.tag = Command::BUILT_IN;

			return *this;
		}

		std::deque<std::string> Command::show()
		{
			std::deque<std::string> args;

			switch(tag)
			{
				case Command::BUILT_IN:
					args.push_back(builtInToString(data.built_in));
					break;
				case Command::PROGRAM:
					args = data.program->show();
					break;
			}

			return args;
		}
}

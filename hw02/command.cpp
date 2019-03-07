#include "command.h"

#include <deque>
#include <string>

namespace shell
{
	Command::~Command()
	{
		switch(tag){
			case BUILT_IN:
				data.built_in.~BuiltIn();
				break;
			case PROGRAM:
				delete data.program;
				break;
		}
	}

	Command& Command::operator=(Command&& cmd)
	{
		tag = cmd.tag;
		switch(tag)
		{
			case BUILT_IN:
				data.built_in = cmd.data.built_in;
				break;
			case PROGRAM:
				data.program = cmd.data.program;
				break;
			default:
				break;
		}

		cmd.tag = Command::BUILT_IN;
		cmd.data.program = nullptr;

		return *this;
	}

	std::deque<std::string> Command::show() const
	{
		std::deque<std::string> args;

		switch(tag)
		{
			case Command::BUILT_IN:
				args.push_back(data.built_in.toString());
				break;
			case Command::PROGRAM:
				args = data.program->show();
				break;
		}

		return args;
	}

	Command makeError(const std::string& what)
	{
		Command error;
		error.tag = Command::BUILT_IN;
		error.data.built_in = BuiltIn(BuiltIn::ERROR);
		error.data.built_in.data.what = what;
		return error;
	}
}

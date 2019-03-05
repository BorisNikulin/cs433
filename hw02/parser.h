#pragma once

#include "command.h"

#include <string>

namespace shell
{
	Command parseCommand(const std::string cmdStr);
}

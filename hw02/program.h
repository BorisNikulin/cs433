#pragma once

#include <unistd.h>

#include <deque>
#include <string>

namespace shell
{
	class Program
	{
		public:
			bool runInBackground = false;
			int in = STDIN_FILENO;
			int out = STDOUT_FILENO;
			int err = STDERR_FILENO;

			virtual ~Program() = default;

			virtual void run() = 0;
			virtual std::deque<std::string> show() = 0;
	};
}

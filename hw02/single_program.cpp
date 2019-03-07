#include "single_program.hpp"

#include <iostream>
#include <cerrno>
#include <cstdlib>

#include <unistd.h>
#include <sys/wait.h>

namespace shell
{
	SingleProgram::~SingleProgram()
	{
		char** currentArg = prog;
		while(*currentArg)
		{
			delete[] *currentArg;
			++currentArg;
		}

		delete[] prog;
	}

	void SingleProgram::run() const
	{
		pid_t pid = fork();

		if(pid == 0)
		{
			dup2(in, STDIN_FILENO);
			dup2(out, STDOUT_FILENO);
			dup2(err, STDERR_FILENO);

			execvp(prog[0], prog);

			std::cerr << "exec error: " << errno << "\n";
			std::exit(errno);
		}
		else if(pid > 0)
		{
			if(!runInBackground)
			{
				waitpid(pid, nullptr, 0);
			}
		}
		else
		{
			std::cerr << "fork error: " << errno << "\n";
		}
	}

	std::deque<std::string> SingleProgram::show()
	{
		char** currentArg = prog;
		std::deque<std::string> args;

		while(*currentArg)
		{
			args.push_back(std::string(*currentArg));
			++currentArg;
		}

		if(runInBackground)
		{
			args.push_back("&");
		}

		return args;
	}
}

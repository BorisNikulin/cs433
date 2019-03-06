#pragma once

#include <unistd.h>

#include <deque>
#include <string>

namespace shell
{
	/// Class that represents a runnable program.
	/**
	 * Program encapsulates a job.
	 * The job does not have to be a single program.
	 * A job may be multiple programs sequenced with pipes or sequentially.
	 *
	 * The Program can be run in the background and or have it's standard streams redirected.
	 *
	 * \sa SingleProgram
	 */
	class Program
	{
		public:
			bool runInBackground = false;
			int in = STDIN_FILENO;
			int out = STDOUT_FILENO;
			int err = STDERR_FILENO;

			virtual ~Program() = default;

			/// Execute the job.
			virtual void run() const = 0; // consider removing const
			/// Obtains a command in the form of a list of space separated elements for displaying.
			virtual std::deque<std::string> show() = 0;
	};
}

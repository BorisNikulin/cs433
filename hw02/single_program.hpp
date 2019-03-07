#pragma once

#include "program.h"
#include "util.hpp"

#include <iterator>
#include <deque>
#include <string>
#include <cstring>

namespace shell
{

	/// A Program consisting of one and only one executable.
	/**
	 * The class encapsulates a single executable such as `echo foo`.
	 *
	 * This class forms the basis for any executable commands.
	 */
	class SingleProgram : public Program
	{
		private:
			char** prog;

		public:

			/// Construct using a list of string arguments and the number of arguments.
			/**
			 * The constructor takes a \p start and \p end pair of iterators to a value type of string
			 * and then a size that corresponds to the number of elements in the range.
			 * Thus, the size must be equal to `std::distance(start, end)`.
			 *
			 * \param start starting forward iterator to std::string.
			 * \param end ending forward iterator to std::string.
				* \param size number of elements in the range [\p start, \p end)
			 */
			template<typename ForwardIt, typename size_type>
			SingleProgram(
				IterWithValueType<ForwardIt, std::string> start,
				// both args cannot use SFINAE (and luckily don't have to) due to inability to infer ForwardIt
				ForwardIt end,
				size_type size);

			virtual ~SingleProgram() override;

			/// Execute the single program.
			virtual void run() const override;
			/// Obtains a command in the form of a list of space separated elements for displaying.
			virtual std::deque<std::string> show() override;
	};

	template<typename ForwardIt, typename size_type>
	SingleProgram::SingleProgram(
		IterWithValueType<ForwardIt, std::string> start,
		ForwardIt end,
		size_type size)
	{
		prog = new char*[size + 1];
		char** currentArg = prog;
		for(; start != end; ++start)
		{
			*currentArg = new char[start->size() + 1];
			strcpy(*currentArg, start->c_str());
			++currentArg;
		}

		*currentArg = nullptr;
	}

}

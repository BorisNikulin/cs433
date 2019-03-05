#pragma once

#include "program.h"

#include <type_traits>
#include <iterator>
#include <deque>
#include <string>
#include <cstring>

namespace shell
{
	template<typename Iter, typename T>
	using IterWithValueType =
		typename std::enable_if<
			std::is_same<
				typename std::iterator_traits<Iter>::value_type,
				T
			>::value,
			Iter
		>::type;

	class SingleProgram : public Program
	{
		public:
			char** prog;

		public:

			template<typename ForwardIt, typename size_type>
			SingleProgram(
				IterWithValueType<ForwardIt, std::string> start,
				ForwardIt end,
				size_type size);

			virtual ~SingleProgram() override;

			virtual void run() const override;
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

		currentArg = nullptr;
	}

}

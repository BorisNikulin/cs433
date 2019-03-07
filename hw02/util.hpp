#pragma once

#include <type_traits>
#include <iterator>

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
}

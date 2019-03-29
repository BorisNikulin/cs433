#pragma once

#include <cstddef>

namespace sim
{
	/// Class for keeping track of sums and the number of additions for use with calculating averages.
	template<typename T, typename CountType = size_t>
	struct SumCount
	{
		using sum_type = T;
		using count_type = CountType;

		sum_type sum;
		count_type count;

		SumCount(T initSum, CountType initCount = 1) : sum(initSum), count(initCount)
		{}

		SumCount(SumCount&) = default;
		SumCount(SumCount&&) = default;
		SumCount& operator=(SumCount&) = default;
	};

	template<typename T, typename CountType, typename U>
	SumCount<T, CountType> operator+(SumCount<T, CountType>& lhs, SumCount<U, CountType>& rhs)
	{
		return SumCount<T, CountType>(lhs.sum + rhs.sum, lhs.count + rhs.count);
	}

	template<typename T, typename CountType, typename U>
	SumCount<T, CountType> operator+(SumCount<T, CountType>& lhs, SumCount<U, CountType>&& rhs)
	{
		return SumCount<T, CountType>(lhs.sum + rhs.sum, lhs.count + rhs.count);
	}

	template<typename T, typename CountType, typename U>
	SumCount<T, CountType>& operator+=(SumCount<T, CountType>& lhs, SumCount<U, CountType>& rhs)
	{
		lhs.sum = lhs.sum + rhs.sum;
		lhs.count = lhs.count + rhs.count;
		return lhs;
	}

	template<typename T, typename CountType>
	SumCount<T, CountType>& operator+=(SumCount<T, CountType>& lhs, SumCount<T, CountType>&& rhs)
	{
		lhs.sum = lhs.sum + rhs.sum;
		lhs.count = lhs.count + rhs.count;
		return lhs;
	}

	template<typename T, typename CountType, typename U>
	SumCount<T, CountType>& operator+=(SumCount<T, CountType>& lhs, U& rhs)
	{
		lhs.sum = lhs.sum + rhs;
		lhs.count = lhs.count + 1;
		return lhs;
	}
	template<typename T, typename CountType, typename U>
	SumCount<T, CountType>& operator+=(SumCount<T, CountType>& lhs, U&& rhs)
	{
		lhs.sum = lhs.sum + rhs;
		lhs.count = lhs.count + 1;
		return lhs;
	}
}

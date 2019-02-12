#pragma once

#include <iterator>
#include <utility>

namespace ds
{
	namespace algorithm
	{
		// constucts a max heap where the comp func indicates which is lesser
		// or in c++ fancy talk: comp satisfies the requirements of Compare
		// https://en.cppreference.com/w/cpp/named_req/Compare
		template<
			typename RandomAccessIterator,
			typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
		auto makeHeap(RandomAccessIterator first, RandomAccessIterator last,
				Compare comp = Compare{})
		-> void
		{
			auto lastIx = std::distance(first, last) - 1;
			auto start = (lastIx - 1) / 2;
			while(start >= 0)
			{
				auto root = start;
				auto child = 2 * root + 1;
				bool done = false;
				while(child <= lastIx && !done)
				{
					if(child + 1 <= lastIx && comp(first[child], first[child + 1]))
					{
						++child;
					}

					if(comp(first[root], first[child]))
					{
						auto tmp(std::move_if_noexcept(first[root]));
						first[root] = std::move_if_noexcept(first[child]);
						first[child] = std::move_if_noexcept(tmp);

						root = child;
						child = 2 * root + 1;
					}
					else
					{
						done = true;
					}
				}
				--start;
			}
		}

		// Pushes the element at last - 1 onto the max heap.
		// comp must satisfy Compare
		template<
			typename RandomAccessIterator,
			typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
		auto pushHeap(RandomAccessIterator first, RandomAccessIterator last,
				Compare comp = Compare{})
		-> void
		{
			auto lastIx = std::distance(first, last) - 1;
			auto parent = (lastIx - 1) / 2;
			while(parent >= 0 && comp(first[parent], first[lastIx]))
			{
				auto tmp(std::move_if_noexcept(first[parent]));
				first[parent] = std::move_if_noexcept(first[lastIx]);
				first[lastIx] = std::move_if_noexcept(tmp);

				lastIx = parent;
				parent = (lastIx - 1) / 2;
			}
		}

		// Pops the element at first off the max heap.
		// such that the first element is placed at last - 1
		// and [first, last - 1) is a max heap.
		// comp must satisfy Compare
		template<
			typename RandomAccessIterator,
			typename Compare = std::less<typename std::iterator_traits<RandomAccessIterator>::value_type>>
		auto popHeap(RandomAccessIterator first, RandomAccessIterator last,
				Compare comp = Compare{})
		-> void
		{
			auto lastIx = std::distance(first, last) - 1;
			if(lastIx > 0)
			{
				auto tmp(std::move_if_noexcept(first[0]));
				first[0] = std::move_if_noexcept(first[lastIx]);
				first[lastIx] = std::move_if_noexcept(tmp);

				auto parent = typename std::iterator_traits<RandomAccessIterator>::difference_type(0);
				auto child =  parent * 2 + 1;
				auto parentLargerThanChild = true;

				while(child < lastIx && parentLargerThanChild)
				{
					if(child + 1 < lastIx && comp(first[child], first[child + 1]))
					{
						++child;
					}

					if(comp(first[parent], first[child]))
					{
						auto tmp(std::move_if_noexcept(first[parent]));
						first[parent] = std::move_if_noexcept(first[child]);
						first[child] = std::move_if_noexcept(tmp);

						parent = child;
						child = parent * 2 + 1;
					}
					else
					{
						parentLargerThanChild = false;
					}
				}
			}
		}
	}
}

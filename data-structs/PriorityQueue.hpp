#pragma once

#include "Algorithm.hpp"
#include "Vector.hpp"

#include <functional>
#include <memory>

namespace ds { namespace container
{
	// NB: The priority queue is unstable unlike the stl priority queue.
	// To maintain insertion order with this queue, pair elements
	// with their insertion order and pass a compare that takes
	// into account that ordering when elements are equal.
	//
	// (or I could figure out how to make it stable but I don't have time)
	//
	// See https://en.cppreference.com/w/cpp/container/priority_queue
	// for template parameters.
	// Container must also have take_back and give_back.
	// The front element of the PriorityQueue is the max element.
	template<
		typename T,
		typename Container = Vector<T>,
		typename Compare = std::less<typename Container::value_type>
	> class PriorityQueue
	{
		public:
			using container_type = Container;
			using value_compare = Compare;
			using value_type = typename Container::value_type;
			using size_type = typename Container::size_type;
			using reference = typename Container::reference;
			using const_reference = typename Container::const_reference;

		private:
			container_type cont;
			value_compare comp;

		public:
			PriorityQueue() : cont(), comp()
			{}
			explicit PriorityQueue(const value_compare& compare) : cont(), comp(compare)
			{}

			PriorityQueue(const value_compare& compare, const container_type& container)
				: cont(container), comp(compare)
			{ algorithm::makeHeap(cont.begin(), cont.end(), comp); }

			PriorityQueue(const value_compare& compare, container_type&& container)
				: cont(std::move(container)), comp(compare)
			{ algorithm::makeHeap(cont.begin(), cont.end(), comp); }

			PriorityQueue(const PriorityQueue& other) : cont(other.cont), comp(std::move(other.comp))
			{}
			PriorityQueue(PriorityQueue&& other)
				: cont(std::move(other.cont)), comp(std::move(other.comp))
			{}

			auto data() -> Container&
			{ return cont; }
			auto data() const -> const Container&
			{ return cont; }

			auto operator=(const PriorityQueue& rhs) -> PriorityQueue&;
			auto operator=(PriorityQueue&& rhs) -> PriorityQueue&;

			auto top() const -> const_reference
			{ return cont.front(); }

			// valid because cont must satisfy Container
			auto empty() const -> bool
			{ return cont.empty(); }
			auto size() const -> size_type
			{ return cont.size(); }

			auto push(const value_type& val) -> void
			{
				cont.push_back(val);
				algorithm::pushHeap(cont.begin(), cont.end(), comp);
			}

			auto push(value_type&& val) -> void
			{
				cont.push_back(std::move_if_noexcept(val));
				algorithm::pushHeap(cont.begin(), cont.end(), comp);
			}

			auto pop() -> void
			{
				algorithm::popHeap(cont.begin(), cont.end(), comp);
				cont.pop_back();
			}

			auto give(std::unique_ptr<value_type> uptr) -> void
			{
				cont.give_back(std::move(uptr));
				algorithm::pushHeap(cont.begin(), cont.end(), comp);
			}

			auto take() -> std::unique_ptr<value_type>
			{
				algorithm::popHeap(cont.begin(), cont.end(), comp);
				return cont.take_back();
			}
	};
} }

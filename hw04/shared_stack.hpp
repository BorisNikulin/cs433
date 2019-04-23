#pragma once

#include <cstddef>
#include <utility>

#include <pthread.h>

namespace concurrent
{
	/// Fixed sized thread safe stack.
	template<typename T, size_t SIZE>
	class SharedStack
	{
		public:
			using value_type = T;
			using size_type = size_t;

		private:
			// see std::aligned_storage
			// i want to try doing it all manually though
			alignas(T) unsigned char buffer[SIZE * sizeof(T)];
			 size_type top;

		public:
			SharedStack() : top(0)
			{}

			~SharedStack();

			// see https://whereswalden.com/tag/stdaligned_storage/
			SharedStack(const SharedStack& other) = delete;

			/// In place constructs and pushes on to the sack, blocking if full.
			template<typename ...Args>
			void emplace(Args&&... args);

			/// Pops the top element and returns it, blocking if empty.
			T pop();
	};

	template<typename T, size_t SIZE>
	SharedStack<T, SIZE>::~SharedStack()
	{
		for(size_t i = 0; i < top; ++i)
		{
			reinterpret_cast<T*>(buffer[i * sizeof(T)])->~T();
		}
	}

	template<typename T, size_t SIZE>
	template<typename ...Args>
	void SharedStack<T, SIZE>::emplace(Args&&... args)
	{
		new(&buffer[top * sizeof(T)]) T(std::forward<Args>(args)...);
		++top;
	}

	template<typename T, size_t SIZE>
	T SharedStack<T, SIZE>::pop()
	{
		--top;
		T elem = *reinterpret_cast<T*>(&buffer[top * sizeof(T)]);
		return elem;
	}
}

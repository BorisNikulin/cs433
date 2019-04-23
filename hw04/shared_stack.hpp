#pragma once

#include <algorithm>
#include <cstddef>
#include <iostream>
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

			pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
			pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;
			pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;

			void printContents();

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
	void SharedStack<T, SIZE>::printContents()
	{
		using namespace std;

		T* tBuffer = reinterpret_cast<T*>(buffer);

		cout << "[";
		if(top == 1)
		{
			cout << tBuffer[0];
		}
		else if(top > 1)
		{
			cout << tBuffer[0];
			for_each(tBuffer + 1, tBuffer + top, [](const T& val){ cout <<  ", " << val; });
		}
		cout << "]";
	}

	template<typename T, size_t SIZE>
	template<typename ...Args>
	void SharedStack<T, SIZE>::emplace(Args&&... args)
	{
		using namespace std;

		//UniqueLock{&mutex};
		pthread_mutex_lock(&mutex);

		cout << pthread_self() << ": ";

		while(top >= SIZE)
		{
			cout << "waiting on notFull\n";
			pthread_cond_wait(&notFull, &mutex);
			cout << pthread_self() << ": ";
		}

		new(&buffer[top * sizeof(T)]) T(std::forward<Args>(args)...);

		T* tBuffer = reinterpret_cast<T*>(buffer);

		cout << "inserted "
			<< tBuffer[top]
			<< ", buffer contains ";
		++top;
		printContents();
		cout << "\n";


		pthread_cond_signal(&notEmpty);
		pthread_mutex_unlock(&mutex);
	}

	template<typename T, size_t SIZE>
	T SharedStack<T, SIZE>::pop()
	{
		using namespace std;

		//UniqueLock{&mutex};
		pthread_mutex_lock(&mutex);

		cout << pthread_self() << ": ";

		while(top <= 0)
		{
			cout << "waiting on notEmpty\n";
			pthread_cond_wait(&notEmpty, &mutex);
			cout << pthread_self() << ": ";
		}

		T* tBuffer = reinterpret_cast<T*>(buffer);

		cout << "popped "
			<< tBuffer[top]
			<< ", buffer contains ";
		--top;
		printContents();
		cout << "\n";

		T elem = tBuffer[top];

		pthread_cond_signal(&notFull);
		pthread_mutex_unlock(&mutex);

		return elem;
	}
}

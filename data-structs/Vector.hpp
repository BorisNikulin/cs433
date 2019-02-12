#pragma once

#include "./../compat/compat.hpp"

#include <algorithm>
#include <cstddef>
#include <cctype>
#include <iterator>
#include <memory>
#include <sstream>

namespace ds
{
	namespace container
	{
		template<typename T>
		class Vector
		{
			public:
				using value_type = T;
				using reference = T&;
				using const_reference = const T&;
				using pointer = T*;
				using const_pointer = const T*;
				using iterator = T*;
				using const_iterator = const T*;
				using reverse_iterator = std::reverse_iterator<iterator>;
				using const_reverse_iterator = std::reverse_iterator<const_iterator>;
				using difference_type = ptrdiff_t;
				using size_type = size_t;

			private:
				T* __arr;
				size_type __size;
				size_type __capacity;

				void rangeCheck(size_type n) const;

			public:
				Vector(std::initializer_list<T>);
				Vector(size_type n = 10) : __arr(new T[n]), __size(0), __capacity(n)
				{}
				Vector(const Vector& v)
				{ *this = v; }
				Vector(Vector&& v) noexcept : __arr(nullptr)
				{ *this = std::move(v); }
				//TODO? add more of the ctor's
				~Vector()
				{ delete[] __arr; }

				Vector& operator=(const Vector&);
				Vector& operator=(Vector&&) noexcept;

				iterator begin() noexcept
				{ return __arr; }
				const_iterator begin() const noexcept
				{ return __arr; }
				const_iterator cbegin() const noexcept
				{ return __arr; }
				iterator end() noexcept
				{ return __arr + __size; }
				const_iterator end() const noexcept
				{ return __arr + __size; }
				const_iterator cend() const noexcept
				{ return __arr + __size; }

				reverse_iterator rbegin() noexcept
				{ return std::reverse_iterator<iterator>(end()); }
				const_reverse_iterator rbegin() const noexcept
				{ return std::reverse_iterator<const_iterator>(end()); }
				const_reverse_iterator crbegin() const noexcept
				{ return std::reverse_iterator<const_iterator>(end()); }
				reverse_iterator rend() noexcept
				{ return std::reverse_iterator<iterator>(begin()); }
				const_reverse_iterator rend() const noexcept
				{ return std::reverse_iterator<iterator>(begin()); }
				const_reverse_iterator crend() const noexcept
				{ return std::reverse_iterator<iterator>(begin()); }

				size_type size() const noexcept
				{ return __size; }
				size_type capacity() const noexcept
				{ return __capacity; }
				void resize(size_type n);
				void reserve(size_type n);
				void shrink_to_fit()
				{ resize(__size); }
				bool empty() const noexcept
				{ return __size == 0; }

				reference operator[](size_type n);
				const_reference operator[](size_type n) const;
				reference at(size_type n);
				const_reference at(size_type n) const;
				reference front()
				{ return (*this)[0]; }
				const_reference front() const
				{ return (*this)[0]; }
				reference back()
				{ return (*this)[__size - 1]; }
				const_reference back() const
				{ return (*this)[__size - 1]; }

				void clear() noexcept
				{ __size = 0; }
				iterator insert(const_iterator pos, const T& value);
				iterator insert(const_iterator pos, T&& value);
				//TODO? add more of the insert overloads
				iterator erase(const_iterator pos);
				iterator erase(const_iterator first, const_iterator last);
				void push_back(const T& value);
				void push_back(T&& value);
				void pop_back()
				{ --__size; }

				auto give_back(std::unique_ptr<value_type> uptr) -> void;
				auto take_back() -> std::unique_ptr<value_type>;

				//TODO comparison ops
		};

		template<typename T>
		Vector<T>::Vector(std::initializer_list<T> l)
		{
			if(l.size() > 0)
			{
				__arr = new T[l.size()];
				__size = l.size();
				__capacity = l.size();
				std::copy(l.begin(), l.end(), begin());
			}
			else
			{
				__arr = new T[10];
				__capacity = 10;
			}
		}

		template<typename T>
		void Vector<T>::rangeCheck(size_type n) const
		{
			if (n >= __size)
			{
				std::ostringstream what;
				what << "index " << n << " is not in range [0," << __size << ")";
				throw std::out_of_range(what.str());
			}
		}

		template<typename T>
		void Vector<T>::resize(Vector<T>::size_type n)
		{
			if(n > __capacity)
			{
				T* newArr = new T[n];
				iterator endIt;
				if(__size <= n)
				{
					endIt = end();
				}
				else
				{
					endIt = begin() + n;
				}
				newArr = std::copy(begin(), endIt, newArr);
				delete[] __arr;
				__arr = newArr;
				__capacity = n;
			}
			__size = n;
		}

		template<typename T>
		void Vector<T>::reserve(size_type n)
		{
			if(n > __capacity)
			{
				// TODO fix the hack
				// temporary fixes are permanent
				size_type beforeSize = __size;
				resize(n);
				__size = beforeSize;
			}
		}


		template<typename T>
		Vector<T>& Vector<T>::operator=(const Vector<T>& v)
		{
			if(__capacity >= v.__capacity)
			{
				std::copy(v.begin(), v.end(), begin());
			}
			else
			{
				__arr = new T[v.__size];
				std::copy(v.begin(), v.end(), begin());
				__capacity = v.__capacity;
			}
			__size = v.__size;

			return *this;
		}

		template<typename T>
		Vector<T>& Vector<T>::operator=(Vector<T>&& v) noexcept
		{
			if(this != &v)
			{
				delete[] __arr;

				__arr = v.__arr ;
				__capacity = v.__capacity;
				__size = v.__size;

				v.__arr = nullptr;
				v.__capacity = 0;
				v.__size = 0;
			}
			return *this;
		}

		template<typename T>
		typename Vector<T>::reference Vector<T>::operator[](size_type n)
		{
			return __arr[n];
		}

		template<typename T>
		typename Vector<T>::const_reference Vector<T>::operator[](size_type n) const
		{
			return __arr[n];
		}

		template<typename T>
		typename Vector<T>::reference Vector<T>::at(size_type n)
		{
			rangeCheck(n);
			return __arr[n];
		}

		template<typename T>
		typename Vector<T>::const_reference Vector<T>::at(size_type n) const
		{
			rangeCheck(n);
			return __arr[n];
		}

		template<typename T>
		typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, const T& value)
		{
			iterator retIt;
			if(pos == end())
			{
				push_back(value);
				retIt = const_cast<iterator>(pos);
			}
			else
			{
				if(__size >= __capacity)
				{
					__capacity *= 2;
					T* newArr = new T[__capacity];
					auto endIt = std::copy(cbegin(), pos, newArr);
					*endIt = value;
					retIt = endIt;
					++endIt;
					std::copy(pos, cend(), endIt);
					__arr = newArr;
				}
				else
				{
					std::copy_backward(pos, cend(), end() + 1);
					retIt = const_cast<iterator>(pos);
				}
				++__size;
			}
			return retIt;
		}

		template<typename T>
		typename Vector<T>::iterator Vector<T>::insert(const_iterator pos, T&& value)
		{
			iterator retIt;
			if(pos == end())
			{
				push_back(value);
				retIt = const_cast<iterator>(pos);
			}
			else
			{
				if(__size >= __capacity)
				{
					__capacity *= 2;
					T* newArr = new T[__capacity];
					auto endIt = std::copy(cbegin(), pos, newArr);
					*endIt = std::move_if_noexcept(value);
					retIt = endIt;
					++endIt;
					std::copy(pos, cend(), endIt);
					__arr = newArr;
				}
				else
				{
					std::copy_backward(pos, cend(), end() + 1);
					retIt = const_cast<iterator>(pos);
				}
				++__size;
			}
			return retIt;
		}


		template<typename T>
		void Vector<T>::push_back(const T& value)
		{
			if(__size >= __capacity)
			{
				__capacity *= 2;
				T* newArr = new T[__capacity];
				std::copy(begin(), end(), newArr);
				newArr[__size] = value;
				__arr = newArr;
			}
			else
			{
				__arr[__size] = value;
			}
			++__size;
		}

		template<typename T>
		void Vector<T>::push_back(T&& value)
		{
			if(__size >= __capacity)
			{
				__capacity *= 2;
				T* newArr = new T[__capacity];
				std::copy(begin(), end(), newArr);
				newArr[__size] = std::move_if_noexcept(value);
				__arr = newArr;
			}
			else
			{
				__arr[__size] = value;
			}
			++__size;
		}

		template<typename T>
		auto Vector<T>::give_back(std::unique_ptr<value_type> uptr) -> void
		{
			push_back(*uptr.release());
		}

		template<typename T>
		auto Vector<T>::take_back() -> std::unique_ptr<value_type>
		{
			auto ret = compat::make_unique<value_type>(back());
			pop_back();
			return ret;
		}

		template<typename T>
		typename Vector<T>::iterator Vector<T>::erase(const_iterator pos)
		{
			std::copy_backward(pos + 1, cend(), end() - 1);
			--__size;
			return const_cast<iterator>(pos);
		}

		template<typename T>
		typename Vector<T>::iterator Vector<T>::erase(const_iterator first, const_iterator last)
		{
			ptrdiff_t erasedCount = last - first;
			std::copy_backward(first, cend(), end() - erasedCount);
			__size -= erasedCount;
			return const_cast<iterator>(first);
		}
	}
}

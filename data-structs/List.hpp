#pragma once

#include <iterator>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <type_traits>

namespace ds
{
	namespace container
	{
		template<typename T>
		class List
		{
			public:
				using value_type = T;
				using reference = T&;
				using const_reference = const T&;
				using pointer = T*;
				using const_pointer = const T*;
				using size_type = size_t;

			private:
				struct Node
				{
					T data;
					Node* prev;
					Node* next;
				};

				class ConstIterator;

				class Iterator
				{
					private:
						Node* cur;

					public:
						using value_type = T;
						using difference_type = void;
						using pointer = T*;
						using reference = T&;
						using iterator_category = std::bidirectional_iterator_tag;

						friend class ConstIterator;

						Iterator() : cur(nullptr)
						{}
						Iterator(Node* cur) : cur(cur)
						{}

						auto operator=(Iterator rhs) -> Iterator&;

						auto operator==(Iterator rhs) const -> bool
						{ return cur == rhs.cur; }
						auto operator!=(Iterator rhs) const -> bool
						{ return !(*this == rhs); }

						auto operator++() -> Iterator&;
						auto operator--() -> Iterator&;
						auto operator*() -> reference
						{ return cur->data; }
						auto operator->() -> pointer
						{ return &cur->data; }
				};

				class ConstIterator
				{
					private:
						const Node* cur;

					public:
						using value_type = const T;
						using difference_type = void;
						using pointer = const T*;
						using reference = const T&;
						using iterator_category = std::bidirectional_iterator_tag;

						friend auto List<T>::insert(ConstIterator pos, const T& val) -> Iterator;
						friend auto List<T>::insert(ConstIterator pos, T&& val) -> Iterator;
						friend auto List<T>::erase(ConstIterator pos) -> Iterator;

						ConstIterator() : cur(nullptr)
						{}
						ConstIterator(const Node* cur) : cur(cur)
						{}
						ConstIterator(Iterator it) : cur(it.cur)
						{}

						auto operator=(ConstIterator rhs) -> ConstIterator&;

						auto operator==(ConstIterator rhs) const -> bool
						{ return cur == rhs.cur; }
						auto operator!=(ConstIterator rhs) const -> bool
						{ return !(*this == rhs); }

						auto operator++() -> ConstIterator&;
						auto operator--() -> ConstIterator&;
						auto operator*() -> reference
						{ return cur->data; }
						auto operator->() -> pointer
						{ return &cur->data; }
				};

				size_type __size;
				Node* head;
				Node* tail;

			public:
				using iterator = Iterator;
				using const_iterator = ConstIterator;
				using reverse_iterator = std::reverse_iterator<iterator>;
				using const_reverse_iterator = std::reverse_iterator<const_iterator>;

				List() : __size(0), head(nullptr), tail(nullptr)
				{}
				List(std::initializer_list<T> il);
				List(const List& l) : head(nullptr), tail(nullptr)
				{ *this = l; }
				List(List&& l) : head(nullptr), tail(nullptr)
				{ *this = std::move(l); }
				~List();

				List& operator=(const List&);
				List& operator=(List&&);

				bool empty() const noexcept
				{ return __size == 0; }
				size_type size() const noexcept
				{ return __size; }

				auto begin() noexcept -> Iterator
				{ return Iterator(head); }
				auto begin() const noexcept -> ConstIterator
				{ return cbegin(); }
				auto cbegin() const noexcept -> ConstIterator
				{ return ConstIterator(head); }
				auto end() noexcept -> Iterator
				{ return Iterator(); }
				auto end() const noexcept -> ConstIterator
				{ return cend(); }
				auto cend() const noexcept -> ConstIterator
				{ return ConstIterator(); }

				reference front()
				{ return head->data; }
				const_reference front() const
				{ return head->data; }
				reference back()
				{ return tail->data; }
				const_reference back() const
				{ return tail->data; }

				void push_back(const T& val);
				void push_back(T&& val);
				void pop_back();
				std::unique_ptr<T> take_back();
				void give_back(std::unique_ptr<T> uptr);

				void push_front(const T& val);
				void push_front(T&& val);
				void pop_front();
				std::unique_ptr<T> take_front();
				void give_front(std::unique_ptr<T> uptr);

				auto insert(const_iterator pos, const T& val) -> iterator;
				auto insert(const_iterator pos, T&& val) -> iterator;

				auto erase(const_iterator pos) -> iterator;
		};

		template<typename T>
		List<T>::List(std::initializer_list<T> il) : __size(0), head(nullptr), tail(nullptr)
		{
			for(auto& x : il)
			{
				push_back(std::move_if_noexcept(x));
			}
		}

		template<typename T>
		List<T>::~List()
		{
			if(head)
			{
				Node* cur = head;
				do
				{
					Node* todoDeleteNode = cur;
					cur = cur->next;
					delete todoDeleteNode;
				} while(cur);
			}
		}

		template<typename T>
		auto List<T>::operator=(const List& rhs) -> List<T>&
		{
			this->~List<T>();
			head = tail = nullptr;
			__size = 0; // insert uses push_back on null iter which does a ++__size
			std::copy(rhs.cbegin(), rhs.cend(), std::inserter(*this, begin()));
			return *this;
		}

		template<typename T>
		auto List<T>::operator=(List&& rhs) -> List<T>&
		{
			this->~List<T>();
			__size = rhs.__size;
			head = rhs.head;
			tail = rhs.tail;
			rhs.head = rhs.tail = nullptr;
			rhs.__size = 0;
			return *this;
		}

		template<typename T>
		void List<T>::push_back(const T& val)
		{
			++__size;
			if(!head)
			{
				head = tail = new Node{val, nullptr, nullptr};
			}
			else
			{
				Node* newNode = new Node{val, tail, nullptr};
				tail->next = newNode;
				tail = newNode;
			}
		}

		template<typename T>
		void List<T>::push_back(T&& val)
		{
			++__size;
			if(!head)
			{
				head = tail = new Node{std::move_if_noexcept(val), nullptr, nullptr};
			}
			else
			{
				Node* newNode = new Node{std::move_if_noexcept(val), tail, nullptr};
				tail->next = newNode;
				tail = newNode;
			}
		}

		template<typename T>
		void List<T>::pop_back()
		{
			--__size;
			Node* todoDeleteNode = tail;
			if(tail == head)
			{
				head = tail = nullptr;
			}
			else
			{
				tail = tail->prev;
				tail->next = nullptr;
			}
			delete todoDeleteNode;
		}

		template<typename T>
		std::unique_ptr<T> List<T>::take_back()
		{
			T* val = new T(std::move_if_noexcept(tail->data));
			std::unique_ptr<T> ret(val);
			Node* todoDeleteNode = tail;
			--__size;

			if(tail == head)
			{
				head = tail = nullptr;
			}
			else
			{
				tail = tail->prev;
				tail->next = nullptr;
			}

			delete todoDeleteNode;
			return std::move(ret);
		}

		template<typename T>
		void List<T>::give_back(std::unique_ptr<T> uptr)
		{
			T& val = *uptr.release();
			Node* newNode = new Node{std::move_if_noexcept(val), nullptr, nullptr};
			val.~T();
			++__size;

			if(!head)
			{
				head = tail = newNode;
			}
			else
			{
				newNode->prev = tail;
				newNode->next = nullptr;
				tail->next = newNode;
				tail = newNode;
			}
		}

		template<typename T>
		auto List<T>::push_front(const T& val) -> void
		{
			++__size;
			if(!head)
			{
				head = tail = new Node{val, nullptr, nullptr};
			}
			else
			{
				Node* newNode = new Node{val, nullptr, head};
				head->prev = newNode;
				head = newNode;
			}
		}

		template<typename T>
		auto List<T>::push_front(T&& val) -> void
		{
			++__size;
			if(!head)
			{
				head = tail = new Node{std::move_if_noexcept(val), nullptr, nullptr};
			}
			else
			{
				Node* newNode = new Node{std::move_if_noexcept(val), nullptr, head};
				head->prev = newNode;
				head = newNode;
			}
		}

		template<typename T>
		auto List<T>::pop_front() -> void
		{
			--__size;
			Node* todoDeleteNode = head;
			if(head == tail)
			{
				head = tail = nullptr;
			}
			else
			{
				head = head->next;
				head->prev = nullptr;
			}
			delete todoDeleteNode;
		}

		template<typename T>
		auto List<T>::take_front() -> std::unique_ptr<T>
		{
			T* val = new T(std::move_if_noexcept(tail->data));
			std::unique_ptr<T> ret(val);
			Node* todoDeleteNode = tail;
			--__size;

			if(tail == head)
			{
				head = tail = nullptr;
			}
			else
			{
				head = head->next;
				head->prev = nullptr;
			}

			delete todoDeleteNode;
			return std::move(ret);
		}

		template<typename T>
		auto List<T>::give_front(std::unique_ptr<T> uptr) -> void
		{
			T& val = *uptr.release();
			Node* newNode = new Node{std::move_if_noexcept(val), nullptr, nullptr};
			val.~T();
			++__size;

			if(!head)
			{
				head = tail = newNode;
			}
			else
			{
				newNode->next = head;
				newNode->prev = nullptr;
				head->prev = newNode;
				head = newNode;
			}
		}

		template<typename T>
		auto List<T>::insert(const_iterator pos, const T& val) -> iterator
		{
			Node* newNode;
			if(pos == cend())
			{
				push_back(val);
				newNode = tail;
			}
			else
			{
				++__size;
				Node* insertPoint = const_cast<Node*>(pos.cur);
				newNode =  new Node{val, insertPoint->prev, insertPoint};
				if(!insertPoint->prev)
				{
					head = newNode;
				}
				insertPoint->prev->next = newNode;
				insertPoint->prev = newNode;
			}
			return Iterator(newNode);
		}

		template<typename T>
		auto List<T>::insert(const_iterator pos, T&& val) -> iterator
		{
			Node* newNode;
			if(pos == cend())
			{
				push_back(std::move_if_noexcept(val));
				newNode = tail;
			}
			else
			{
				++__size;
				Node* insertPoint = const_cast<Node*>(pos.cur);
				newNode =  new Node{std::move_if_noexcept(val), insertPoint->prev, insertPoint};
				if(!insertPoint->prev)
				{
					head = newNode;
				}
				insertPoint->prev->next = newNode;
				insertPoint->prev = newNode;
			}
			return Iterator(newNode);
		}

		template<typename T>
		auto List<T>::erase(const_iterator pos) -> iterator
		{
			--__size;
			Node* erasedNode = const_cast<Node*>(pos.cur);
			Iterator next(erasedNode->next);
			if(erasedNode->prev)
			{
				erasedNode->prev->next = erasedNode->next;
			}
			else
			{
				head = erasedNode->next;
			}
			if(erasedNode->next)
			{
				erasedNode->next->prev = erasedNode->prev;
			}
			else
			{
				tail = erasedNode->prev;
			}
			delete erasedNode;
			return next;
		}

		template<typename T>
		auto List<T>::Iterator::operator=(Iterator rhs) -> Iterator&
		{
			cur = rhs.cur;
			return *this;
		}

		template<typename T>
		auto List<T>::Iterator::operator++() -> Iterator&
		{
			cur = cur->next;
			return *this;
		}

		template<typename T>
		auto List<T>::Iterator::operator--() -> Iterator&
		{
			cur = cur->prev;
			return *this;
		}

		template<typename T>
		auto List<T>::ConstIterator::operator=(ConstIterator rhs) -> ConstIterator&
		{
			cur = rhs.cur;
			return *this;
		}

		template<typename T>
		auto List<T>::ConstIterator::operator++() -> ConstIterator&
		{
			cur = cur->next;
			return *this;
		}

		template<typename T>
		auto List<T>::ConstIterator::operator--() -> ConstIterator&
		{
			cur = cur->prev;
			return *this;
		}
	}
}

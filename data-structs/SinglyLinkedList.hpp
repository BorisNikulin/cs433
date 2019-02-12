#pragma once

#include <memory>

namespace ds
{
	namespace container
	{
		template<typename E>
		class SinglyLinkedList
		{
			private:
				class Node
				{
					public:
						Node(std::unique_ptr<Node>& next, std::unique_ptr<E>&& data) : next(std::move(next)), data(std::move(data))
						{}
						Node(std::unique_ptr<Node>& next, std::unique_ptr<E>& data) : next(std::move(next)), data(std::move(data))
						{}
						Node(std::unique_ptr<Node>&& next, std::unique_ptr<E>& data) : next(std::move(next)), data(std::move(data))
						{}
						Node(std::unique_ptr<Node>&& next, std::unique_ptr<E>&& data) : next(std::move(next)), data(std::move(data))
						{}
						std::unique_ptr<Node> next;
						std::unique_ptr<E> data;
				};

				class ConstIterator;

				class Iterator
				{
					private:
						// 2 levels of indirection but it makes pointing to the actual begining easier
						std::unique_ptr<Node> * cur;
						static const std::unique_ptr<Node> END;

					public:
						using value_type = E;
						using difference_type = void;
						using pointer = E*;
						using reference = E&;
						using iterator_category = std::forward_iterator_tag;

						friend class ConstIterator;
						friend Iterator SinglyLinkedList<E>::insert(Iterator pos, const E&);
						friend Iterator SinglyLinkedList<E>::end();
						friend ConstIterator SinglyLinkedList<E>::cend() const;

						Iterator() : cur(&END)
						{}
						Iterator(std::unique_ptr<Node>* cur) : cur(cur)
						{}

						bool operator==(Iterator it) const
						{ return *this->cur == *it.cur; }
						bool operator!=(Iterator it) const
						{ return !(*this == it); }

						Iterator& operator++();
						reference operator*();
						pointer operator->();
				};

				// copy paste programming
				class ConstIterator
				{
					private:
						// 2 levels of indirection but it makes pointing to the actual begining easier
						const std::unique_ptr<Node>* cur;
						static const std::unique_ptr<Node> END;

					public:
						using value_type = const E;
						using difference_type = void;
						using pointer = const E*;
						using reference = const E&;
						using iterator_category = std::forward_iterator_tag;

						friend ConstIterator SinglyLinkedList<E>::cend() const;

						ConstIterator() : cur(&END)
						{}
						ConstIterator(const std::unique_ptr<Node>* cur) : cur(cur)
						{}
						ConstIterator(Iterator it) : cur(it.cur)
						{}

						bool operator==(ConstIterator it) const
						{ return *this->cur == *it.cur; }
						bool operator!=(ConstIterator it) const
						{ return !(*this == it); }

						ConstIterator& operator++();
						reference operator*();
						pointer operator->();
				};
				std::unique_ptr<Node> head;

			public:
				using value_type = E;
				using pointer = E*;
				using reference = E&;
				using iterator = Iterator;
				using const_iterator = ConstIterator;

				SinglyLinkedList() : head(std::unique_ptr<Node>(nullptr))
				{}
				SinglyLinkedList(const SinglyLinkedList<E>& l)
				{ *this = l; }
				SinglyLinkedList(SinglyLinkedList<E>&& l)
				{ *this = std::move(l); }
				SinglyLinkedList(std::initializer_list<E>);

				SinglyLinkedList<E>& operator=(const SinglyLinkedList<E>&);
				SinglyLinkedList<E>& operator=(SinglyLinkedList<E>&& l) noexcept;

				void push(const E& val);
				void push(E&& val);
				E& peek();
				void pop();
				std::unique_ptr<E> take();
				Iterator insert(Iterator pos, const E&);
				Iterator insert(Iterator pos, E&&);
				bool isEmpty() const
				{ return !static_cast<bool>(this->head); }

				Iterator begin()
				{ return Iterator(&this->head); }
				ConstIterator begin() const
				{ return cbegin(); }
				ConstIterator cbegin() const
				{ return ConstIterator(&this->head); }
				Iterator end()
				{ return Iterator(const_cast<std::unique_ptr<Node>*>(&Iterator::END)); }
				ConstIterator end() const
				{ return cend(); }
				ConstIterator cend() const
				{ return ConstIterator(&ConstIterator::END); }
		};

		template<typename E>
		SinglyLinkedList<E>::SinglyLinkedList(std::initializer_list<E> vals)
		{
			std::copy(vals.begin(), vals.end(), std::inserter(*this, this->begin()));
		}

		template<typename E>
		SinglyLinkedList<E>& SinglyLinkedList<E>::operator=(const SinglyLinkedList& l)
		{
			std::copy(l.begin(), l.end(), std::inserter(*this, this->begin()));
			return *this;
		}

		template<typename E>
		SinglyLinkedList<E>& SinglyLinkedList<E>::operator=(SinglyLinkedList&& l) noexcept
		{
			this->head = std::move(l.head);
			return *this;
		}

		template<typename E>
		void SinglyLinkedList<E>::push(const E& val)
		{
			Node* newHead = new Node(this->head, std::unique_ptr<E>(new E(val)));
			this->head = std::unique_ptr<Node>(newHead);
		}

		template<typename E>
		void SinglyLinkedList<E>::push(E&& val)
		{
			Node* newHead = new Node(this->head, std::unique_ptr<E>(new E(std::move_if_noexcept(val))));
			this->head = std::unique_ptr<Node>(newHead);
		}

		template<typename E>
		E& SinglyLinkedList<E>::peek()
		{
			return *this->head->data;
		}

		template<typename E>
		void SinglyLinkedList<E>::pop()
		{
			this->head = std::unique_ptr<Node>(std::move(this->head->next));
		}

		template<typename E>
		std::unique_ptr<E> SinglyLinkedList<E>::take()
		{
			std::unique_ptr<E> ret(std::move(this->head->data));
			this->pop();
			return ret;
		}

		template<typename E>
		typename SinglyLinkedList<E>::Iterator SinglyLinkedList<E>::insert(Iterator pos, const E& val)
		{
			Node* newNode = new Node(*pos.cur, std::unique_ptr<E>(new E(val)));
			*pos.cur = std::unique_ptr<Node>(newNode);
			return pos;
		}

		template<typename E>
		typename SinglyLinkedList<E>::Iterator SinglyLinkedList<E>::insert(Iterator pos, E&& val)
		{
			Node* newNode = new Node(std::move_if_noexcept(*pos.cur), std::unique_ptr<E>(new E(val)));
			*pos.cur = std::unique_ptr<Node>(newNode);
			return pos;
		}

		template<typename E>
		const std::unique_ptr<typename SinglyLinkedList<E>::Node> SinglyLinkedList<E>::Iterator::END = std::unique_ptr<Node>(nullptr);

		template<typename E>
		const std::unique_ptr<typename SinglyLinkedList<E>::Node> SinglyLinkedList<E>::ConstIterator::END = std::unique_ptr<Node>(nullptr);

		template<typename E>
		typename SinglyLinkedList<E>::Iterator& SinglyLinkedList<E>::Iterator::operator++()
		{
			this->cur = &(*this->cur)->next;
			return *this;
		}

		template<typename E>
		typename SinglyLinkedList<E>::Iterator::reference SinglyLinkedList<E>::Iterator::operator*()
		{
			return *(*this->cur)->data;
		}

		template<typename E>
		typename SinglyLinkedList<E>::Iterator::pointer SinglyLinkedList<E>::Iterator::operator->()
		{
			return (*this->cur)->data.get();
		}
		template<typename E>

		typename SinglyLinkedList<E>::ConstIterator& SinglyLinkedList<E>::ConstIterator::operator++()

		{
			this->cur = &(*this->cur)->next;
			return *this;
		}

		template<typename E>
		typename SinglyLinkedList<E>::ConstIterator::reference SinglyLinkedList<E>::ConstIterator::operator*()
		{
			return *(*this->cur)->data;
		}

		template<typename E>
		typename SinglyLinkedList<E>::ConstIterator::pointer SinglyLinkedList<E>::ConstIterator::operator->()
		{
			return (*this->cur)->data.get();
		}
	}
}

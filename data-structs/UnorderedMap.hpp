#pragma once

#include "SinglyLinkedList.hpp"
#include "Vector.hpp"

#include <cmath>
#include <functional>
#include <utility>

namespace ds
{
	namespace container
	{
		template<
			typename Key,
			typename T,
			typename Hash = std::hash<Key>,
			typename Predicate = std::equal_to<Key>>
		class UnorderedMap
		{
			public:
				using key_type = Key;
				using mapped_type = T;
				using value_type = std::pair<const key_type, mapped_type>;
				using hasher = Hash;
				using key_equal = Predicate;
				using reference = value_type&;
				using const_reference = const value_type&;
				using pointer = value_type*;
				using const_pointer = const value_type*;
				using size_type = size_t;

			private:
				class ConstIterator;

				class Iterator
				{
					private:
						using VecIt = typename Vector<SinglyLinkedList<UnorderedMap::value_type>>::iterator;
						using ListIt = typename SinglyLinkedList<UnorderedMap::value_type>::iterator;

						VecIt vecIt;
						VecIt vecEndMinus1;
						ListIt listIt;

					public:
						using value_type = UnorderedMap::value_type;
						using difference_type = void;
						using pointer = UnorderedMap::pointer;
						using reference = UnorderedMap::reference;
						using iterator_category = std::forward_iterator_tag;

						Iterator() : vecIt(VecIt{}), vecEndMinus1(VecIt{}), listIt(ListIt{})
						{}
						Iterator(VecIt vecIt, VecIt vecEnd, ListIt listIt) : vecIt(vecIt), vecEndMinus1(vecEnd-1), listIt(listIt)
						{}

						friend class ConstIterator;

						bool operator==(Iterator it) const
						{ return vecIt == it.vecIt && listIt == it.listIt; }
						bool operator!=(Iterator it) const
						{ return !(*this == it); }

						Iterator& operator++();
						reference operator*()
						{ return *listIt; }
						pointer operator->()
						{ return &*listIt; }
				};

				class ConstIterator
				{
					private:
						using ConstVecIt = typename Vector<SinglyLinkedList<UnorderedMap::value_type>>::const_iterator;
						using ConstListIt = typename SinglyLinkedList<UnorderedMap::value_type>::const_iterator;

						ConstVecIt vecIt;
						ConstVecIt vecEndMinus1;
						ConstListIt listIt;

					public:
						using value_type = const UnorderedMap::value_type;
						using difference_type = void;
						using pointer = const value_type*;
						using reference = const value_type&;
						using iterator_category = std::forward_iterator_tag;

						ConstIterator() : vecIt(ConstVecIt{}), vecEndMinus1(ConstVecIt{}), listIt(ConstListIt{})
						{}
						ConstIterator(ConstVecIt vecIt, ConstVecIt vecEnd, ConstListIt listIt) : vecIt(vecIt), vecEndMinus1(vecEnd-1), listIt(listIt)
						{}
						ConstIterator(Iterator it) : vecIt(it.vecIt), vecEndMinus1(it.vecEndMinus1), listIt(it.listIt)
						{}

						bool operator==(ConstIterator it) const
						{ return vecIt == it.vecIt && listIt == it.listIt; }
						bool operator!=(ConstIterator it) const
						{ return !(*this == it); }

						ConstIterator& operator++();
						reference operator*()
						{ return *listIt; }
						pointer operator->()
						{ return &*listIt; }
				};

				Vector<SinglyLinkedList<value_type>> __map;
				size_type __size;
				hasher hash;
				key_equal equal;
				//TODO make loadFactorThreshold changable
				const float loadFactorThreshold = 3.0 / 4.0;

				std::function<bool(const value_type&)> makeEqualFunc(const key_type& lhs) const noexcept
				{ return [this, &lhs](const value_type& rhs){ return equal(lhs, rhs.first); }; }

			public:
				using iterator = Iterator;
				using const_iterator = ConstIterator;

				explicit UnorderedMap(
						size_type n = 16,
						const hasher& hf = hasher(),
						const key_equal eql = key_equal())
					: __map(Vector<SinglyLinkedList<value_type>>(n)),
					__size(0),
					hash(hf),
					equal(eql)
				{ __map.resize(n); }
				UnorderedMap(
						std::initializer_list<value_type> il,
						size_type n = 16,
						const hasher& hf = hasher(),
						const key_equal eql = key_equal());
				UnorderedMap(const UnorderedMap& um)
				{ *this = um; }
				UnorderedMap(UnorderedMap&& um) noexcept
				{ *this = std::move(um); }

				UnorderedMap& operator=(const UnorderedMap& um);
				UnorderedMap& operator=(UnorderedMap&& um) noexcept;

				bool empty() const noexcept
				{ return __size == 0; }
				size_type size() const noexcept
				{ return __size; }
				size_type bucket_count() const noexcept
				{ return __map.capacity(); }

				iterator begin() noexcept;
				const_iterator begin() const noexcept
				{ return cbegin(); }
				const_iterator cbegin() const noexcept;
				iterator end() noexcept
				{ return Iterator(__map.end() - 1, __map.end(), (__map.end() - 1)->end()); }
				const_iterator end() const noexcept
				{ return cend(); }
				const_iterator cend() const noexcept
				{ return ConstIterator(__map.cend() - 1, __map.cend(), __map.cend()->cend()); }

				void rehash(size_type count);

				std::pair<iterator, bool> insert(const value_type& val);
				std::pair<iterator, bool> insert(value_type&& val);
				iterator insert(const_iterator, const value_type& val)
				{ return insert(val).first; }
				iterator insert(const_iterator, value_type&& val)
				{ return insert(std::move(val)).first; }

				mapped_type& operator[](const key_type& k);
				mapped_type& operator[](key_type&& k);
				mapped_type& at(const key_type& k);
				const mapped_type& at(const key_type& k) const;
				bool contains(const key_type& key) const;
		};

		template<typename Key, typename T, typename Hash, typename Pred>
		UnorderedMap<Key, T, Hash, Pred>::UnorderedMap(
				std::initializer_list<value_type> li,
				size_type n,
				const hasher& hf,
				const key_equal eql)
		{
			__map = Vector<SinglyLinkedList<value_type>>(
					std::max(static_cast<size_type>(std::ceil(li.size() / loadFactorThreshold)), n));
			hash = hf;
			equal = eql;
			for(auto& a : li)
			{
				insert(std::move_if_noexcept(a));
			}
		}


		template<typename Key, typename T, typename Hash, typename Pred>
		UnorderedMap<Key, T, Hash, Pred>&  UnorderedMap<Key, T, Hash, Pred>::operator=(const UnorderedMap& um)
		{
			__map = um.__map;
			__size = um.__size;
			hash = um.hash;
			equal = um.equal;
			return *this;
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		UnorderedMap<Key, T, Hash, Pred>&  UnorderedMap<Key, T, Hash, Pred>::operator=(UnorderedMap&& um) noexcept
		{
			__map = std::move(um.__map);
			__size = um.__size;
			um.__size = 0;
			hash = um.hash;
			equal = um.equal;
			return *this;
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		void UnorderedMap<Key, T, Hash, Pred>::rehash(size_type count)
		{
			if(count >= __size / loadFactorThreshold)
			{
				UnorderedMap<Key, T, Hash, Pred> newMap(count, hash, equal);
				std::copy(begin(), end(), std::inserter(newMap, newMap.begin()));
				*this = std::move(newMap);
			}
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		std::pair<typename UnorderedMap<Key, T, Hash, Pred>::iterator, bool>  UnorderedMap<Key, T, Hash, Pred>::insert(const value_type& val)
		{
			// rehashing can still occur if the added element is already in the map
			// since the if check happens whatever the value of val
			if(static_cast<float>(__size + 1) / __map.capacity() > loadFactorThreshold)
			{
				rehash(__map.capacity() * 2);
			}
			size_type bucketIx = hash(val.first) % __map.capacity();
			SinglyLinkedList<value_type>& bucket = __map[bucketIx];
			auto insertIt = std::find_if(bucket.begin(), bucket.end(), makeEqualFunc(val.first));
			if(insertIt == bucket.end())
			{
				++__size;
				bucket.push(val);
				return {Iterator(__map.begin() + bucketIx, __map.end(), bucket.begin()), true};
			}
			else
			{
				return {end(), false};
			}
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		std::pair<typename UnorderedMap<Key, T, Hash, Pred>::iterator, bool> UnorderedMap<Key, T, Hash, Pred>::insert(value_type&& val)
		{
			if(static_cast<float>(__size) / __map.capacity() > loadFactorThreshold)
			{
				rehash(__map.capacity() * 2);
			}
			size_type bucketIx = hash(val.first) % __map.capacity();
			SinglyLinkedList<value_type>& bucket = __map[bucketIx];
			auto insertIt = std::find_if(bucket.begin(), bucket.end(), makeEqualFunc(val.first));
			if(insertIt == bucket.end())
			{
				++__size;
				bucket.push(std::move_if_noexcept(val));
				return {Iterator(__map.begin() + bucketIx, __map.end(), bucket.begin()), true};
			}
			else
			{
				return {end(), false};
			}
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		typename UnorderedMap<Key, T, Hash, Pred>::mapped_type& UnorderedMap<Key, T, Hash, Pred>::operator[](const key_type& key)
		{
			auto& bucket = __map[hash(key) % __map.capacity()];
			auto it = std::find_if(bucket.begin(), bucket.end(), makeEqualFunc(key));
			if(it == bucket.end())
			{
				//there is fancy in place construction and other high level really advanced c++ I could do
				//but im not at that level so ill just default construct a T and then give out a ref
				bucket.push({std::move_if_noexcept(key), T()});
				return bucket.peek().second;
			}
			return it->second;
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		typename UnorderedMap<Key, T, Hash, Pred>::mapped_type& UnorderedMap<Key, T, Hash, Pred>::operator[](key_type&& key)
		{
			auto& bucket = __map[hash(key) % __map.capacity()];
			auto it = std::find_if(bucket.begin(), bucket.end(), makeEqualFunc(key));
			if(it == bucket.end())
			{
				//there is fancy in place construction and other high level really advanced c++ I could do
				//but im not at that level so ill just default construct a T and then give out a ref
				bucket.push({std::move_if_noexcept(key), T()});
				return bucket.peek().second;
			}
			return it->second;
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		typename UnorderedMap<Key, T, Hash, Pred>::mapped_type& UnorderedMap<Key, T, Hash, Pred>::at(const key_type& key)
		{
			auto& bucket = __map[hash(key) % __map.capacity()];
			auto it = std::find_if(bucket.begin(), bucket.end(), makeEqualFunc(key));
			if(it != bucket.end())
			{
				return it->second;
			}
			std::ostringstream what;
			what << "key " << key << " does not exist in the map";
			throw std::out_of_range(what.str());
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		const typename UnorderedMap<Key, T, Hash, Pred>::mapped_type& UnorderedMap<Key, T, Hash, Pred>::at(const key_type& key) const
		{
			auto& bucket = __map[hash(key) % __map.capacity()];
			auto it = std::find_if(bucket.begin(), bucket.end(), makeEqualFunc(key));
			if(it != bucket.end())
			{
				return it->second;
			}
			std::ostringstream what;
			what << "key \"" << key << "\" does not exist in the map";
			throw std::out_of_range(what.str());
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		bool UnorderedMap<Key, T, Hash, Pred>::contains(const key_type& key) const
		{
			auto& bucket = __map[hash(key) % __map.capacity()];
			auto it = std::find_if(bucket.begin(), bucket.end(), makeEqualFunc(key));
			return it != bucket.end();
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		typename UnorderedMap<Key, T, Hash, Pred>::iterator UnorderedMap<Key,T, Hash, Pred>::begin() noexcept
		{
			auto& bucket = __map[0];
			Iterator it(__map.begin(), __map.end(), bucket.begin());
			if(bucket.begin() != bucket.end())
			{
				return it;
			}
			return ++it;
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		typename UnorderedMap<Key, T, Hash, Pred>::const_iterator UnorderedMap<Key,T, Hash, Pred>::cbegin() const noexcept
		{
			auto& bucket = __map[0];
			ConstIterator it(__map.begin(), __map.end(), bucket.begin());
			if(bucket.begin() != bucket.end())
			{
				return it;
			}
			return ++it;
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		typename UnorderedMap<Key, T, Hash, Pred>::iterator& UnorderedMap<Key, T, Hash, Pred>::Iterator::operator++()
		{
			if(listIt != vecIt->end())
			{
				++listIt;
			}

			while(vecIt != vecEndMinus1 && listIt == vecIt->end())
			{
				++vecIt;
				listIt = vecIt->begin();
			}

			return *this;
		}

		template<typename Key, typename T, typename Hash, typename Pred>
		typename UnorderedMap<Key, T, Hash, Pred>::const_iterator& UnorderedMap<Key, T, Hash, Pred>::ConstIterator::operator++()
		{
			if(listIt != vecIt->end())
			{
				++listIt;
			}

			while(vecIt != vecEndMinus1 && listIt == vecIt->end())
			{
				++vecIt;
				listIt = vecIt->begin();
			}

			return *this;
		}
	}
}

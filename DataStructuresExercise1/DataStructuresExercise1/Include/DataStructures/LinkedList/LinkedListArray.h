#ifndef INCLUDE_DATASTRUCTURES_LINKEDLIST_LINKEDLISTARRAY__H
#define INCLUDE_DATASTRUCTURES_LINKEDLIST_LINKEDLISTARRAY__H

#include <vector>

namespace containers
{
	template<typename ElementType>
	class LinkedListArray
	{
		// Friend Classes
		friend class LinkedNode;

	public:
		// Subclasses
		class LinkedNode
		{
			// Friend Classes
			friend class LinkedListArray;

		private:
			// Error Messages
			static constexpr auto DEREFERENCED_INVALID_ITERATOR = "Can't dereference a dangling iterator!";
			static constexpr auto INCREMENTED_INVALID_ITERATOR = "Can't increment a dangling iterator!";
			static constexpr auto DECREMENTED_INVALID_ITERATOR = "Can't decrement a dangling iterator!";

			// Members
			LinkedListArray& container;
			unsigned int dataIndex;

			// C'tors
			LinkedNode(const LinkedListArray& container, unsigned int dataIndex) : container(const_cast<LinkedListArray&>(container)), dataIndex(dataIndex) {}

		public:
			// C'tors
			LinkedNode(const LinkedNode&) = default;
			LinkedNode(LinkedNode&&) = default;

			// Assignment 
			LinkedNode& operator=(const LinkedNode&) = default;
			LinkedNode& operator=(LinkedNode&&) = default;

			// Iterator Methods
			inline auto& operator*() 
			{
				if (dataIndex == NONEXISTENT_ELEMENT) throw DEREFERENCED_INVALID_ITERATOR;
				return container.elements[dataIndex]; 
			}

			inline const auto& operator*() const 
			{
				if (dataIndex == NONEXISTENT_ELEMENT) throw DEREFERENCED_INVALID_ITERATOR;
				return container.elements[dataIndex]; 
			}

			inline auto& operator++() 
			{
				if (dataIndex == NONEXISTENT_ELEMENT) throw INCREMENTED_INVALID_ITERATOR;
				return dataIndex = container.elementNexts[dataIndex]; 
			}

			auto operator++(int)
			{
				LinkedNode before = *this;
				++(*this);

				return before;
			}

			inline auto& operator--() 
			{
				if (dataIndex == NONEXISTENT_ELEMENT) throw DECREMENTED_INVALID_ITERATOR;
				return dataIndex = container.elementPrevs[dataIndex]; 
			}

			auto operator--(int)
			{
				LinkedNode before = *this;
				--(*this);

				return before;
			}

			inline auto operator!=(const LinkedNode& other) const
			{
				return &container != &other.container || dataIndex != other.dataIndex;
			}

			inline auto operator==(const LinkedNode& other) const { return !(*this != other); }
		};

		// Constructors
		LinkedListArray(size_t capacity = 256) : elements(capacity), elementNexts(capacity), elementPrevs(capacity) { Clear(); }
		LinkedListArray(const LinkedListArray& other) : LinkedListArray(other.Capacity()) { *this = other; }
		LinkedListArray(LinkedListArray&& other) = default;

		template<typename BeginIteratorType, typename EndIteratorType>
		LinkedListArray(unsigned int capacity, const BeginIteratorType& begin, const EndIteratorType& end) : LinkedListArray(capacity) { Append(begin, end); }

		// Destructor
		~LinkedListArray() = default;

		// Assignment Operator Methods
		LinkedListArray& operator=(LinkedListArray&& other) = default;
		inline auto& operator=(const LinkedListArray& other)
		{
			return &other != this ? Clear().Append(other) : *this;
		}

		// Getters
		inline auto Size() const { return size; }
		inline auto IsEmpty() const { return size == 0; }
		inline auto Capacity() const { return elements.size(); }
		inline auto begin() const { return LinkedNode(*this, firstIndex); }
		inline auto end() const { return LinkedNode(*this, NONEXISTENT_ELEMENT); }
		inline auto Contains(const ElementType& element) const
		{
			for (const auto& current : *this) if (current == element) return true;
			return false;
		}
		inline auto& First() 
		{
			if (IsEmpty()) throw GET_ELEMENT_WHEN_EMPTY;
			return elements[firstIndex]; 
		}

		inline const auto& First() const 
		{
			if (IsEmpty()) throw GET_ELEMENT_WHEN_EMPTY;
			return elements[firstIndex]; 
		}

		inline auto& Last() 
		{
			if (IsEmpty()) throw GET_ELEMENT_WHEN_EMPTY;
			return elements[lastIndex]; 
		}

		inline const auto& Last() const 
		{
			if (IsEmpty()) throw GET_ELEMENT_WHEN_EMPTY;
			return elements[lastIndex];
		}

		// List Manipulation
		auto& Append(ElementType element)
		{
			if (size == Capacity()) throw ADD_ELEMENT_WHEN_FULL;

			if (size == 0)
			{
				firstIndex = lastIndex = freeIndex;
			}
			else
			{
				elementNexts[lastIndex] = freeIndex;
				elementPrevs[freeIndex] = lastIndex;
				lastIndex = freeIndex;
			}

			elements[lastIndex] = std::move(element);
			freeIndex = elementNexts[freeIndex];
			elementNexts[lastIndex] = NONEXISTENT_ELEMENT;
			++size;

			return *this;
		}

		auto& Append(const LinkedListArray& other)
		{
			unsigned int count = 0;
			unsigned int otherSize = other.size;

			for (const auto& otherData : other)
			{
				Append(otherData);

				if (++count == otherSize)
				{
					break;
				}
			}

			return *this;
		}

		template<typename BeginIteratorType, typename EndIteratorType>
		auto& Append(const BeginIteratorType& begin, const EndIteratorType& end)
		{
			while (begin != end)
			{
				Append(*(begin++));
			}
		}

		template<typename Any>
		inline auto& Prepend(Any&& other)
		{
			return *this = LinkedListArray<ElementType>(Capacity()).Append(std::forward<Any>(other)).Append(*this);
		}

		template<typename BeginIteratorType, typename EndIteratorType>
		auto& Prepend(const BeginIteratorType& begin, const EndIteratorType& end)
		{
			return *this = LinkedListArray<ElementType>(Capacity()).Append(begin, end).Append(*this);
		}

		template<typename Container>
		inline auto& operator+=(const Container& other) { return Append(other); }

		template<typename Container>
		inline auto operator+(const Container& other) { return LinkedListArray(*this).Append(other); }

		auto& Remove(const ElementType& element)
		{
			auto currentIndex = firstIndex;

			while (currentIndex != NONEXISTENT_ELEMENT)
			{
				if (elements[currentIndex] == element)
				{
					if (currentIndex == lastIndex)
					{
						lastIndex = elementPrevs[lastIndex];
						elementNexts[lastIndex] = NONEXISTENT_ELEMENT;
					}

					if (currentIndex == firstIndex)
					{
						firstIndex = elementNexts[firstIndex];
						elementPrevs[firstIndex] = NONEXISTENT_ELEMENT;
					}

					elementNexts[elementPrevs[currentIndex]] = elementNexts[currentIndex];
					elementPrevs[elementNexts[currentIndex]] = elementPrevs[currentIndex];

					auto toDelete = currentIndex;
					currentIndex = elementNexts[currentIndex];
					DeleteIndex(toDelete);
				}
				else
				{
					currentIndex = elementNexts[currentIndex];
				}
			}

			return *this;
		}

		auto& RemoveFirst()
		{
			if (IsEmpty()) throw REMOVED_ELEMENT_WHEN_EMPTY;
			int toDelete = firstIndex;
			firstIndex = elementNexts[firstIndex];
			elementPrevs[firstIndex] = NONEXISTENT_ELEMENT;
			DeleteIndex(toDelete);

			return *this;
		}

		auto& RemoveLast()
		{
			if (IsEmpty()) throw REMOVED_ELEMENT_WHEN_EMPTY;
			int toDelete = lastIndex;
			lastIndex = elementPrevs[lastIndex];
			elementNexts[lastIndex] = NONEXISTENT_ELEMENT;
			DeleteIndex(toDelete);

			return *this;
		}

		auto& MakeEmpty() { return Clear(); }
		auto& Clear()
		{
			freeIndex = 0;
			size = 0;
			firstIndex = lastIndex = NONEXISTENT_ELEMENT;

			for (unsigned int index = 0; index < Capacity(); index++)
			{
				elementNexts[index] = index + 1;
				elementPrevs[index] = NONEXISTENT_ELEMENT;
			}

			return *this;
		}

	private:
		// Error Messages
		static constexpr auto GET_ELEMENT_WHEN_EMPTY = "Can't get an element when the list is empty!";
		static constexpr auto REMOVED_ELEMENT_WHEN_EMPTY = "Can't remove an element when the list is empty!";
		static constexpr auto ADD_ELEMENT_WHEN_FULL = "Not enough capacity to add an element to the list!";

		// Consts
		static constexpr auto NONEXISTENT_ELEMENT = -1;

		// Data Members
		std::vector<ElementType> elements;
		std::vector<int> elementNexts;
		std::vector<int> elementPrevs;
		unsigned int size;
		int firstIndex;
		int lastIndex;
		int freeIndex;

		// Methods
		void DeleteIndex(int deletedIndex)
		{
			elementNexts[deletedIndex] = freeIndex;
			freeIndex = deletedIndex;
			elements[deletedIndex] = ElementType();
			--size;
		}
	};
}

#endif // !INCLUDE_DATASTRUCTURES_LINKEDLIST_LINKEDLISTARRAY__H

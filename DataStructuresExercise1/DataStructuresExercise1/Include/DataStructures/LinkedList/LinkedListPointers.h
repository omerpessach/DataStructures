#ifndef INCLUDE_DATASTRUCTURES_LINKEDLIST_LINKEDLISTPOINTERS__H
#define INCLUDE_DATASTRUCTURES_LINKEDLIST_LINKEDLISTPOINTERS__H

#include <utility>

namespace containers
{
	template<typename ElementType>
	class LinkedListPointers
	{
	public:
		// Subclasses
		class LinkedNode
		{
			// Friend Classes
			friend class LinkedListPointers;

		private:
			// Error Messages
			static constexpr auto DEREFERENCED_INVALID_ITERATOR = "Can't dereference a dangling iterator!";
			static constexpr auto INCREMENTED_INVALID_ITERATOR = "Can't increment a dangling iterator!";
			static constexpr auto DECREMENTED_INVALID_ITERATOR = "Can't decrement a dangling iterator!";

			// Members
			ElementType data;
			LinkedNode* next;
			LinkedNode* previous;

			// C'tors
			LinkedNode(ElementType data, LinkedNode* next, LinkedNode* previous) : data(data), next(next), previous(previous) {}

		public:
			// C'tors
			LinkedNode(): data(), next(nullptr), previous(nullptr) { }
			LinkedNode(const LinkedNode&) = default;
			LinkedNode(LinkedNode&&) = default;

			// Assignment 
			LinkedNode& operator=(const LinkedNode&) = default;
			LinkedNode& operator=(LinkedNode&&) = default;

			// Iterator Methods
			inline auto& operator*()
			{
				if (*this == dummy) throw DEREFERENCED_INVALID_ITERATOR;
				return data; 
			}

			inline const auto& operator*() const
			{
				if (*this == dummy) throw DEREFERENCED_INVALID_ITERATOR;
				return data; 
			}

			inline auto& operator++()
			{
				if (*this == dummy) throw INCREMENTED_INVALID_ITERATOR;
				return *this = *next; 
			}

			auto operator++(int)
			{
				LinkedNode before = *this;
				++(*this);

				return before;
			}

			inline auto& operator--()
			{
				if (*this == dummy) throw DECREMENTED_INVALID_ITERATOR;
				return *this = *previous; 
			}

			auto operator--(int)
			{
				LinkedNode before = *this;
				--(*this);

				return before;
			}

			inline auto operator!=(const LinkedNode& other) const
			{
				return data != other.data || next != other.next || previous != other.previous;
			}

			inline auto operator==(const LinkedNode& other) const { return !(*this != other); }
		};

		// Constructors
		LinkedListPointers() : first(&dummy), last(&dummy), size(0) { }
		LinkedListPointers(const LinkedListPointers& other) : LinkedListPointers() { *this = other; }
		LinkedListPointers(LinkedListPointers&& other) : LinkedListPointers() { *this = std::move(other); }

		template<typename BeginIteratorType, typename EndIteratorType>
		LinkedListPointers(const BeginIteratorType& begin, const EndIteratorType& end) : LinkedListPointers() { Append(begin, end); }

		// Destructor
		~LinkedListPointers() { Clear(); }

		// Assignment Operator Methods
		inline auto& operator=(LinkedListPointers&& other)
		{
			return this != reinterpret_cast<const LinkedListPointers*>(&other) ? Clear().Append(std::move(other)) : *this;
		}

		inline auto& operator=(const LinkedListPointers& other)
		{
			return this != reinterpret_cast<const LinkedListPointers*>(&other) ? Clear().Append(other) : *this;
		}

		// Getters
		inline auto Size() const { return size; }
		inline auto IsEmpty() const { return size == 0; }
		inline auto begin() const { return *first; }
		inline auto end() const { return dummy; }
		inline auto Contains(const ElementType& element) const 
		{
			for (const auto& current : *this) if (current == element) return true;
			return false;
		}
		inline auto& First()
		{
			if (IsEmpty()) throw GET_ELEMENT_WHEN_EMPTY;
			return first->data; 
		}

		inline const auto& First() const
		{
			if (IsEmpty()) throw GET_ELEMENT_WHEN_EMPTY;
			return first->data; 
		}

		inline auto& Last()
		{
			if (IsEmpty()) throw GET_ELEMENT_WHEN_EMPTY;
			return last->data; 
		}

		inline const auto& Last() const
		{
			if (IsEmpty()) throw GET_ELEMENT_WHEN_EMPTY;
			return last->data; 
		}

		// List Manipulation
		auto& Append(ElementType element)
		{
			if (size == 0)
			{
				LinkedNode* newNode = new LinkedNode(std::move(element), &dummy, &dummy);
				if (!newNode) throw NEW_FAILED;

				first = last = newNode;
			}
			else
			{
				LinkedNode* newNode = new LinkedNode(std::move(element), &dummy, last);
				if (!newNode) throw NEW_FAILED;

				last->next = newNode;
				last = last->next;
			}

			++size;

			return *this;
		}
		
		auto& Append(const LinkedListPointers& other)
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

		auto& Append(LinkedListPointers&& other)
		{
			if (&other == this)
			{
				Append(other);
			}
			else
			{
				if (size == 0)
				{
					first = other.first;
					last = other.last;
				}
				else
				{
					last->next = other.first;
					other.first->previous = last;
				}

				other.first = &dummy;
				size += other.size;
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
			return *this = LinkedListPointers<ElementType>().Append(std::forward<Any>(other)).Append(std::move(*this));
		}

		template<typename BeginIteratorType, typename EndIteratorType>
		auto& Prepend(const BeginIteratorType& begin, const EndIteratorType& end)
		{
			return *this = LinkedListPointers<ElementType>().Append(begin, end).Append(*this);
		}

		template<typename Container>
		inline auto& operator+=(Container&& other){ return Append(std::forward<Container>(other)); }

		template<typename Container>
		inline auto operator+(Container&& other){ return LinkedListPointers(*this).Append(std::forward<Container>(other)); }

		auto& Remove(const ElementType& element)
		{
			auto current = first;

			while (current != &dummy)
			{
				if (current->data == element)
				{
					if (current == last)
					{
						last = current->previous;
						last->next = &dummy;
					}

					if (current == first)
					{
						first = first->next;
						first->previous = &dummy;
					}

					current->previous->next = current->next;
					current->next->previous = current->previous;

					auto toDelete = current;
					current = current->next;
					delete toDelete;
					--size;
				}
				else
				{
					current = current->next;
				}
			}

			return *this;
		}
		
		auto& RemoveFirst()
		{
			if (IsEmpty()) throw REMOVED_ELEMENT_WHEN_EMPTY;
			LinkedNode* toDelete = first;

			first = first->next;
			first->previous = &dummy;

			delete toDelete;
			--size;

			if (size == 0)
			{
				last = &dummy;
			}

			return *this;
		}

		auto& RemoveLast()
		{
			if (IsEmpty()) throw REMOVED_ELEMENT_WHEN_EMPTY;
			LinkedNode* toDelete = last;
			
			last = last->previous;
			last->next = &dummy;

			delete toDelete;
			--size;

			if (size == 0)
			{
				first = &dummy;
			}

			return *this;
		}

		auto& MakeEmpty() { return Clear(); }
		auto& Clear()
		{
			while (first != &dummy)
			{
				auto toDelete = first;
				first = first->next;
				delete toDelete;
			}

			last = &dummy;
			size = 0;

			return *this;
		}

	private:
		// Error Messages
		static constexpr auto NEW_FAILED = "Failed to allocate memory with 'new'!";
		static constexpr auto GET_ELEMENT_WHEN_EMPTY = "Can't get an element when the list is empty!";
		static constexpr auto REMOVED_ELEMENT_WHEN_EMPTY = "Can't remove an element when the list is empty!";

		// Static Data Members
		static LinkedNode dummy;

		// Data Members
		LinkedNode* first;
		LinkedNode* last;
		unsigned int size;
	};
}

template<typename ElementType>
typename containers::LinkedListPointers<ElementType>::LinkedNode containers::LinkedListPointers<ElementType>::dummy;

#endif // !INCLUDE_DATASTRUCTURES_LINKEDLIST_LINKEDLISTPOINTERS__H

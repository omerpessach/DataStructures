#ifndef INCLUDE_DATASTRUCTURES_STACK__H
#define INCLUDE_DATASTRUCTURES_STACK__H

#include "DataStructures/LinkedList/LinkedListPointers.h"

namespace containers
{
	template<typename ElementType, typename UnderlyingContainer = LinkedListPointers<ElementType>>
	class Stack : private UnderlyingContainer
	{
	private:
		// Error Messages
		static constexpr auto GET_TOP_WHEN_EMPTY = "Can't get the top of the stack when it's empty!";
		static constexpr auto POP_WHEN_EMPTY = "Can't pop the stack when it's empty!";
		static constexpr auto PUSH_FAILED = "Failed to push a new element to the stack!";

	public:
		// Constructors
		Stack() = default;
		Stack(const Stack& other) throw (const char*) = default;
		Stack(Stack&& other) throw (const char*) = default;

		// Destructor
		~Stack() = default;

		// Assignment Operator Methods
		Stack& operator=(const Stack& other) = default;
		Stack& operator=(Stack&& other) = default;

		// Getters
		inline auto Size() const { return UnderlyingContainer::Size(); }
		inline auto IsEmpty() const { return UnderlyingContainer::IsEmpty(); }
		inline auto Top() const throw (const char*) 
		{
			if (IsEmpty()) throw GET_TOP_WHEN_EMPTY;
			return UnderlyingContainer::Last(); 
		}

		// Stack Manipulation
		inline auto& Push(ElementType element) throw (const char*) 
		{
			try { return UnderlyingContainer::Append(std::move(element)); }
			catch (const char* error) { throw PUSH_FAILED; }
		}

		inline auto Pop() throw (const char*) 
		{
			if (IsEmpty()) throw POP_WHEN_EMPTY;
			auto temp = Top(); UnderlyingContainer::RemoveLast(); return temp; 
		}

		inline auto& MakeEmpty() { UnderlyingContainer::MakeEmpty(); }
		inline auto& Clear() { UnderlyingContainer::Clear(); }
	};
}


#endif // !INCLUDE_DATASTRUCTURES_STACK__H

#ifndef INCLUDE_DATASTRUCTURES_STACK__H
#define INCLUDE_DATASTRUCTURES_STACK__H

#include "DataStructures/LinkedList/LinkedListPointers.h"

namespace containers
{
	template<typename ElementType, typename UnderlyingContainer = LinkedListPointers<ElementType>>
	class Stack : private UnderlyingContainer
	{
	public:
		// Constructors
		Stack() = default;
		Stack(const Stack& other) = default;
		Stack(Stack&& other) = default;

		// Destructor
		~Stack() = default;

		// Assignment Operator Methods
		Stack& operator=(const Stack& other) = default;
		Stack& operator=(Stack&& other) = default;

		// Getters
		inline auto Size() const { return UnderlyingContainer::Size(); }
		inline auto IsEmpty() const { return UnderlyingContainer::IsEmpty(); }
		inline auto Top() const 
		{
			if (IsEmpty()) throw GET_TOP_WHEN_EMPTY;
			return UnderlyingContainer::Last(); 
		}

		// Stack Manipulation
		inline auto& Push(ElementType element) 
		{
			try { return UnderlyingContainer::Append(std::move(element)); }
			catch (const char*) { throw PUSH_FAILED; }
		}

		inline auto Pop() 
		{
			if (IsEmpty()) throw POP_WHEN_EMPTY;
			auto temp = Top(); UnderlyingContainer::RemoveLast(); return temp; 
		}

		inline auto& MakeEmpty() { return UnderlyingContainer::MakeEmpty(); }
		inline auto& Clear() { return UnderlyingContainer::Clear(); }

	private:
		// Error Messages
		static constexpr auto GET_TOP_WHEN_EMPTY = "Can't get the top of the stack when it's empty!";
		static constexpr auto POP_WHEN_EMPTY = "Can't pop the stack when it's empty!";
		static constexpr auto PUSH_FAILED = "Failed to push a new element to the stack!";
	};
}


#endif // !INCLUDE_DATASTRUCTURES_STACK__H

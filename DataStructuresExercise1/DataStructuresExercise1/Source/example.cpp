
#include "DataStructures/LinkedList/LinkedListPointers.h"
#include "DataStructures/LinkedList/LinkedListArray.h"
#include "DataStructures/Stack.h"

void LinkedListWithPointersExample()
{
	// Create an empty linked list (using pointers)
	containers::LinkedListPointers<int> myList; 

	// Append 3 to the end of the list ([3])
	myList.Append(3); 

	// Append 2 to the end of the list ([3, 2])
	myList.Append(2); 

	// Prepend 5 to the start of the list ([5, 3, 2])
	myList.Prepend(5); 

	// firstNumber = 5
	auto firstNumber = myList.First();

	// lastNumber = 2
	auto lastNumber = myList.Last();
	
	// Create a copy of myList. This will copy and create new pointers with the same data
	containers::LinkedListPointers<int> myListCopy(myList); 

	// You can even do multiple method calls one after the other.
	myListCopy.Append(7).RemoveFirst().Prepend(12);

	// This creates a new list by adding the two lists together
	auto listAddition = myList + myListCopy;

	// You can append lists aswell
	listAddition.Append(myList);

	// This is like Append
	listAddition += myList;

	// Here we appended myListCopy to listAddition without copies, and only by using pointers.
	// So this is O(1) instead of O(n)
	listAddition.Append(std::move(myListCopy));

	// By the way, 'auto' is a way to take the type of the right hand side without actually saying it.
	// The type is still fixed, but automatically deduced by the expression to the right
	auto sum = 0;

	// Go over the list one element at a time
	for (auto currentNumber : myList)
	{
		sum += currentNumber;
	}

	// This is better for when the list is made of heavy to copy items.
	// We can just use references without copying them
	for (const auto& currentNumber : myList)
	{
		sum += currentNumber;
	}

	// Here we do regular & without const, to actually change the elements of the list
	for (auto& currentNumber : myList)
	{
		currentNumber++;
	}
}

void LinkedListWithArrayExample()
{
	// Here we create a linked list that is implemented with an array.
	// The array will be of the fixed size 100.
	containers::LinkedListArray<int> array(100);

	// We can still do everything that we could with the LinkedList of pointers
	array.Append(3);
	array += array;
	for (auto& currentNumber : array)
	{
		currentNumber++;
	}
}

void StackExample()
{
	// Regular usage of stack of ints
	containers::Stack<int> stack;
	stack.Push(3);
	stack.Push(15);
	stack.Push(-7);
	stack.Push(2);
	int poppedValue = stack.Pop(); // 2
	int size = stack.Size();
	stack.Clear();
	bool isEmpty = stack.IsEmpty();

	// The stack is implemented using LinkedListPointers in the background.
	// However, you can change its background data structure like this:
	containers::Stack<int, containers::LinkedListArray<int>> newStack;

	// Don't do it though, they asked us to make the stack unlimited in its size
}

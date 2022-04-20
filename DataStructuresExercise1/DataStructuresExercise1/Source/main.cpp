
#include <iostream>
#include "DataStructures/Stack.h"
#include "DataStructures/LinkedList/LinkedListArray.h"

using namespace containers;

int main()
{
    try
    {
        Stack<int, LinkedListArray<int, 2>> s;
        s.Push(1);
        s.Push(1);
        s.Push(1);
        s.Pop();
        s.Top();
    }
    catch (const char* errorMessage)
    {
        std::cout << errorMessage << "\n";
    }
}

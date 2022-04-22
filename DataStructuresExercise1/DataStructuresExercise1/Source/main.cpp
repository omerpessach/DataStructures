
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include "DataStructures/LinkedList/LinkedListPointers.h"
#include "DataStructures/LinkedList/LinkedListArray.h"
#include "DataStructures/Stack.h"

using namespace std;
using namespace containers;

using Color = bool;
constexpr auto WHITE = true;
constexpr auto BLACK = false;

struct UserInput
{
    struct Pair
    {
        unsigned int source;
        unsigned int destination;
    };

    unsigned int numberOfTowns;
    std::vector<Pair> pairs;
    unsigned int source;
};

auto GetCountryFromUserInput(const UserInput& userInput)
{
    vector<LinkedListPointers<unsigned int>> country(userInput.numberOfTowns);

    for (const auto& pair : userInput.pairs)
    {
        country[pair.source - 1].Append(pair.destination - 1);
    }

    return country;
}

namespace recursive
{
    void GetToTown(const vector<LinkedListPointers<unsigned int>>& country,
                   unsigned int townNumber,
                   vector<Color>& coloredTowns,
                   LinkedListArray<unsigned int>& accessibleTowns)
    {
        coloredTowns[townNumber] = BLACK;
        accessibleTowns.Append(townNumber);

        for (const auto& neighbor : country[townNumber])
        {
            if (coloredTowns[neighbor] == WHITE)
            {
                GetToTown(country, neighbor, coloredTowns, accessibleTowns);
            }
        }
    }
};

namespace iterative
{
    auto GetToTown(const vector<LinkedListPointers<unsigned int>>& country,
                   unsigned int townNumber)
    {
        struct ItemType
        {
            // Enums
            enum class Line
            {
                START,
                LOOP
            };

            // Local Variables
            LinkedListArray<unsigned int>* accessibleTowns = nullptr;
            vector<Color>* coloredTowns = nullptr;
            LinkedListPointers<unsigned int>::LinkedNode neighborIterator;

            // Parameters
            const vector<LinkedListPointers<unsigned int>>* country = nullptr;
            unsigned int townNumber = 0;

            // Line
            Line line = Line::START;
        };

        LinkedListArray<unsigned int> accessibleTowns(country.size());
        vector<Color> coloredTowns(country.size(), WHITE);

        Stack<ItemType> stack;
        stack.Push({ &accessibleTowns, 
                     &coloredTowns, 
                     country[townNumber].begin(),
                     &country, 
                     townNumber, 
                     ItemType::Line::START });

        while (!stack.IsEmpty())
        {
            ItemType current = stack.Pop();

            if (current.line == ItemType::Line::START)
            {
                (*current.coloredTowns)[current.townNumber] = BLACK;
                current.accessibleTowns->Append(current.townNumber);
                current.line = ItemType::Line::LOOP;
            }

            if (current.line == ItemType::Line::LOOP)
            {
                while (current.neighborIterator != (*current.country)[current.townNumber].end() &&
                       (*current.coloredTowns)[*current.neighborIterator] != WHITE)
                {
                    ++current.neighborIterator;
                }

                if (current.neighborIterator != (*current.country)[current.townNumber].end())
                {
                    stack.Push(current);

                    stack.Push({ current.accessibleTowns,
                                 current.coloredTowns,
                                 (*current.country)[*current.neighborIterator].begin(),
                                 current.country,
                                 *current.neighborIterator,
                                 ItemType::Line::START });
                }

            }
        }

        return accessibleTowns;
    }
};

void PrintAccessibleTowns(const LinkedListArray<unsigned int>& accessibleTowns)
{
    for (const auto& accessibleTown : accessibleTowns)
    {
        cout << (accessibleTown + 1) << " ";
    }

    cout << "\n";
}

auto GetUserInput()
{
    // Get the number of towns and the number of pairs seperated by a space
    int numberOfTowns;
    int numberOfPairs;
    cin >> numberOfTowns >> numberOfPairs;
    bool wasInvalid = cin.fail() || numberOfTowns <= 0 || numberOfPairs < 0 || numberOfPairs > pow(numberOfTowns, numberOfTowns);
    cin.ignore(1, '\n');

    // Get the line of town connections
    string townConnectionsInput;
    getline(cin, townConnectionsInput);
    wasInvalid = wasInvalid || cin.fail();

    // Count the number of towns received
    unsigned int numberOfTownsReceived = 0;
    int townNumber;
    stringstream stream(townConnectionsInput);
    while (stream >> townNumber)
    {
        if (townNumber > numberOfTowns || townNumber <= 0)
        {
            wasInvalid = true;
            break;
        }

        numberOfTownsReceived++;
    }

    wasInvalid = wasInvalid || numberOfTownsReceived % 2 != 0 || numberOfTownsReceived / 2 != numberOfPairs;

    // Get the source town
    int source;
    cin >> source;

    stream = stringstream(townConnectionsInput);

    // The college wanted us to first receive all of the input and then check for validity, so we only check now.
    if (wasInvalid || cin.fail() || source > numberOfTowns || source <= 0) throw "invalid input";

    UserInput userInput = {static_cast<unsigned int>(numberOfTowns), std::vector<UserInput::Pair>(numberOfTownsReceived / 2), static_cast<unsigned int>(source)};
    for (auto& pair : userInput.pairs) stream >> pair.source >> pair.destination;

    return userInput;
}

int main()
{
    try
    {
        auto userInput = GetUserInput();
        auto country = GetCountryFromUserInput(userInput);

        cout << "Cities accessible from source city " << userInput.source << " (recursive algorithm): ";

        LinkedListArray<unsigned int> accessibleTowns(country.size());
        vector<Color> coloredTowns(country.size(), WHITE);
        recursive::GetToTown(country, userInput.source - 1, coloredTowns, accessibleTowns);
        PrintAccessibleTowns(accessibleTowns);

        cout << "Cities accessible from source city " << userInput.source << " (iterative algorithm): ";
        PrintAccessibleTowns(iterative::GetToTown(country, userInput.source - 1));
    }
    catch (const char* errorMessage)
    {
        cout << errorMessage << "\n";
    }
}

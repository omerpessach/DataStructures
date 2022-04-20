
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "DataStructures/LinkedList/LinkedListPointers.h"
#include "DataStructures/LinkedList/LinkedListArray.h"
#include "DataStructures/Stack.h"

using namespace std;
using namespace containers;

using Color = bool;
constexpr auto WHITE = true;
constexpr auto BLACK = false;

void AssertTownNumber(size_t numberOfTowns, unsigned int townNumber)
{
    if (townNumber > numberOfTowns || townNumber == 0) throw "Invalid town number!";
}

auto GetCountryFromUser()
{
    unsigned int numberOfTowns;
    unsigned int numberOfPairs;
    string townConnectionsInput;

    cout << "Please enter the number of towns, and the number of pairs (seperated with a space): ";
    cin >> numberOfTowns >> numberOfPairs;
    if (cin.fail()) throw "Invalid input!";
    cin.ignore(1, '\n');

    cout << "Please enter the town connections in pairs, in one line (seperated with a space):\n";
    getline(cin, townConnectionsInput);
    if (cin.fail()) throw "Invalid input!";

    unsigned int numberOfPairsReceived = 0;

    vector<LinkedListPointers<unsigned int>> country(numberOfTowns);

    stringstream stream(townConnectionsInput);

    unsigned int firstTownNumber;
    while (stream >> firstTownNumber)
    {
        AssertTownNumber(numberOfTowns, firstTownNumber);

        unsigned int secondTownNumber;
        if (!(stream >> secondTownNumber)) throw "Invalid input, or you haven't entered an even number of towns!";
        AssertTownNumber(numberOfTowns, secondTownNumber);

        country[firstTownNumber - 1].Append(secondTownNumber - 1);
        ++numberOfPairsReceived;
    }

    if (numberOfPairsReceived != numberOfPairs) throw "Invalid input, or you haven't entered the right amount of pairs!";

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
            LinkedListArray<unsigned int>* accessibleTowns;
            vector<Color>* coloredTowns;
            LinkedListPointers<unsigned int>::LinkedNode neighborIterator;

            // Parameters
            const vector<LinkedListPointers<unsigned int>>* country;
            unsigned int townNumber;

            // Line
            Line line;
        };

        LinkedListArray<unsigned int> accessibleTowns(country.size());
        vector<Color> coloredTowns(country.size(), WHITE);

        Stack<ItemType, LinkedListArray<ItemType>> stack;
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

int main()
{
    try
    {
        auto country = GetCountryFromUser();

        // Get the town number
        unsigned int townToCheck;
        cout << "Please enter the town number to find the accessible towns from it: ";
        cin >> townToCheck;
        if (cin.fail()) throw "Invalid input!";
        AssertTownNumber(country.size(), townToCheck);
        --townToCheck;

        cout << "Accessible towns by recursion: ";

        LinkedListArray<unsigned int> accessibleTowns(country.size());
        vector<Color> coloredTowns(country.size(), WHITE);
        recursive::GetToTown(country, townToCheck, coloredTowns, accessibleTowns);
        PrintAccessibleTowns(accessibleTowns);

        cout << "Accessible towns by iteration: ";
        PrintAccessibleTowns(iterative::GetToTown(country, townToCheck));
    }
    catch (const char* errorMessage)
    {
        cout << errorMessage << "\n";
    }
}

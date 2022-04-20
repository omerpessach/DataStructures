
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
    cin.ignore(1, '\n');

    cout << "Please enter the town connections in pairs, in one line (seperated with a space):\n";
    getline(cin, townConnectionsInput);

    unsigned int numberOfPairsReceived = 0;

    vector<LinkedListPointers<unsigned int>> country(numberOfTowns);

    stringstream stream(townConnectionsInput);

    unsigned int firstTownNumber;
    while (stream >> firstTownNumber)
    {
        AssertTownNumber(numberOfTowns, firstTownNumber);

        unsigned int secondTownNumber;
        if (!(stream >> secondTownNumber)) throw "Invalid input! You haven't entered an even number of towns!";
        AssertTownNumber(numberOfTowns, secondTownNumber);

        country[firstTownNumber - 1].Append(secondTownNumber - 1);
        ++numberOfPairsReceived;
    }

    if (numberOfPairsReceived != numberOfPairs) throw "You haven't entered the right amount of pairs!";

    return country;
}

namespace recursive
{
    auto GetToTown(const vector<LinkedListPointers<unsigned int>>& country,
                   unsigned int townNumber,
                   vector<Color>& coloredTowns,
                   LinkedListArray<unsigned int>& accessibleTowns)
    {
        // TODO: Pessach

        return accessibleTowns;
    }
};

namespace iterative
{
    auto GetToTown(const vector<LinkedListPointers<unsigned int>>& country,
                   unsigned int townNumber)
    {
        struct ItemType
        {
            // TODO: Pessach
        };

        LinkedListArray<unsigned int> accessibleTowns(country.size());
        Stack<ItemType> stack;

        // TODO: Pessach

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
        auto towns = GetCountryFromUser();

        // Get the town number
        unsigned int townToCheck;
        cout << "Please enter the town number to find the accessible towns from it: ";
        cin >> townToCheck;
        AssertTownNumber(towns.size(), townToCheck);
        --townToCheck;

        cout << "Accessible towns by recursion: ";

        LinkedListArray<unsigned int> accessibleTowns(towns.size());
        vector<Color> coloredTowns(towns.size(), WHITE);
        recursive::GetToTown(towns, townToCheck, coloredTowns, accessibleTowns);
        PrintAccessibleTowns(accessibleTowns);

        cout << "Accessible towns by iteration: ";
        PrintAccessibleTowns(iterative::GetToTown(towns, townToCheck));
    }
    catch (const char* errorMessage)
    {
        cout << errorMessage << "\n";
    }
}

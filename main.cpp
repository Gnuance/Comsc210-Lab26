/*
    Lab 25: Which data structure is fastest? In each race, the contestants (a vector, list, and set) will all compete against each other.
        1. There will be four various races! Write a program that will time, record, and report how long it takes each contestant to run these four races:
            a. The first race is READING. The challenge is to read the 20,000 data elements into each of these data structures: vector, list, set
            b. The second race is SORTING. Sort the vector and sort the list. Set the set's value to -1 since a set is already sorted.
            c. The third race is INSERTING. Insert the value "TESTCODE" into the middle of the vector or the middle of the list, or into the set
        2. Use the cool time features of the <chrono> library. You'll just be using a stopwatch on these operations.
    Requirements:
        1. Make your code scalable. It should be simple to add new races.
*/

#include <iostream>
#include <iomanip> // align output
#include <cstdio>  // right align output
#include <chrono>
// required data classes
#include <vector>
#include <list>
#include <set>
#include <fstream>   // file
#include <algorithm> // sort
using namespace std;
using namespace std::chrono; // so chrono:: doesn't have to be used over and over again

// timers to be used in functions. To truly de-couple, they could be initialized inside each function
high_resolution_clock::time_point timerStart;
high_resolution_clock::time_point timerEnd;
duration<double, milli> elapsed;

// individual race functions
vector<double> ReadRace(vector<string> &, list<string> &, set<string> &, const string);
vector<double> SortRace(vector<string> &, list<string> &, set<string> &);
vector<double> InsertRace(vector<string> &, list<string> &, set<string> &, const string);
vector<double> DeleteRace(vector<string> &, list<string> &, set<string> &);

int main()
{
    // initialize data containers to race
    vector<string> vecCodes = {};
    list<string> listCodes = {};
    set<string> setCodes = {};

    // file to read data from
    const string FILE_NAME = "codes.txt";
    // value to insert for race 3
    const string INSERT_CODE = "TESTCODE";

    // off to the races. Record race results in local variables
    vector<double> readRaceResults = ReadRace(vecCodes, listCodes, setCodes, FILE_NAME);
    vector<double> sortRaceResults = SortRace(vecCodes, listCodes, setCodes);
    vector<double> insertRaceResults = InsertRace(vecCodes, listCodes, setCodes, INSERT_CODE);
    vector<double> deleteRaceResults = DeleteRace(vecCodes, listCodes, setCodes);

    // output results
    printf("%10s\t%10s\t%10s\t%10s\t\n", "Operation", "Vector (ms)", "List (ms)", "Set (ms)");
    printf("%10s\t%10f\t%10f\t%10f\t\n", "Read", readRaceResults.at(0), readRaceResults.at(1), readRaceResults.at(2));
    printf("%10s\t%10f\t%10f\t%10f\t\n", "Sort", sortRaceResults.at(0), sortRaceResults.at(1), sortRaceResults.at(2));
    printf("%10s\t%10f\t%10f\t%10f\t\n", "Insert", insertRaceResults.at(0), insertRaceResults.at(1), insertRaceResults.at(2));
    printf("%10s\t%10f\t%10f\t%10f\t\n", "Delete", deleteRaceResults.at(0), deleteRaceResults.at(1), deleteRaceResults.at(2));

    return 0;
}

// read race function
vector<double> ReadRace(vector<string> &vecCodes, list<string> &listCodes, set<string> &setCodes, const string FILE_NAME)
{
    vector<double> results = {};
    ifstream inputFile(FILE_NAME);
    string line = "";

    // guard statement in case input file doesn't open
    if (!inputFile)
    {
        cout << "ERROR: Opening " << FILE_NAME << ": File cannot be opened." << endl;
    }
    // time each container separately and reset cusor
    timerStart = high_resolution_clock::now();
    while (getline(inputFile, line))
    {
        vecCodes.push_back(line); // vector read race
    }
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    // clear eof bit and reset cursor to beginning
    inputFile.clear();
    inputFile.seekg(0);

    timerStart = high_resolution_clock::now();
    while (getline(inputFile, line))
    {
        listCodes.push_back(line); // list read race
    }
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    // clear eof bit and reset cursor to beginning
    inputFile.clear();
    inputFile.seekg(0);

    timerStart = high_resolution_clock::now();
    while (getline(inputFile, line))
    {
        setCodes.insert(line); // set read race
    }
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    // file reading done, close stream
    inputFile.close();

    return results;
}

// sorting race, only vector and list need to be sorted
vector<double> SortRace(vector<string> &vecCodes, list<string> &listCodes, set<string> &setCodes)
{
    vector<double> results = {};

    // vector, then list, then set
    timerStart = high_resolution_clock::now();
    sort(vecCodes.begin(), vecCodes.end());
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    timerStart = high_resolution_clock::now();
    listCodes.sort();
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    results.push_back(-1); // set is already sorted
    return results;
}

// insert race: place insert code into the middle of the container
// set does not allow for manual placement
vector<double> InsertRace(vector<string> &vecCodes, list<string> &listCodes, set<string> &setCodes, const string INSERT_CODE)
{
    vector<double> results = {};
    list<string>::iterator listIter;

    // vector, then list, then set
    timerStart = high_resolution_clock::now();
    vecCodes.insert(vecCodes.begin() + vecCodes.size() / 2, INSERT_CODE);
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    timerStart = high_resolution_clock::now();
    listIter = next(listCodes.begin(), listCodes.size() / 2);
    listCodes.insert(listIter, INSERT_CODE);
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    timerStart = high_resolution_clock::now();
    setCodes.insert(INSERT_CODE);
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    return results;
}

// delete race: delete middle element of containers
vector<double> DeleteRace(vector<string> &vecCodes, list<string> &listCodes, set<string> &setCodes)
{
    // variables for results and iterators for list and set
    vector<double> results = {};
    list<string>::iterator listIter;
    set<string>::iterator setIter;

    // vector, then list, then set
    timerStart = high_resolution_clock::now();
    vecCodes.erase(vecCodes.begin() + vecCodes.size() / 2);
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    timerStart = high_resolution_clock::now();
    listIter = next(listCodes.begin(), listCodes.size() / 2);
    listCodes.erase(listIter);
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    timerStart = high_resolution_clock::now();
    setIter = next(setCodes.begin(), setCodes.size() / 2);
    setCodes.erase(setIter);
    timerEnd = high_resolution_clock::now();
    elapsed = timerEnd - timerStart;
    results.push_back(elapsed.count());

    return results;
}

/* syntax examples:
auto start = high_resolution_clock::now()
auto end = high_resolution_clock::now()
auto duration = duration_cast<milliseconds>(end - start)
duration.count() references elapsed milliseconds
*/
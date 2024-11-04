/*
    Lab 26: Abstract your code from Lab 25. Run the experiment 15 times, and output the average run times of all the races.
        1. You're using a 2-D array to store your data in Lab 25; make it a 3-D array here.
        2. The second cube would hold counter variables and just subtotal the variables that are being recorded in the first cube.

        Design decisions:
            1. To run each race 15 times, I see two options with the way the program is currently written:
                a. Create a heat for each group of races. For ex: heat1 = vector of read, sort, insert, and delete for each container.
                b. Run each race 15 times in a row, accumulate results and store values to display.
                --> Option b should be much easier to implement, but Option a is more in line with OOP principles and
                        is more similar to real life application.
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

    // accumulators & counter for racing averages added for Lab26
    const int NUMBER_OF_HEATS = 15;
    vector<vector<double>> vecResults(4, vector<double>(3, 0));
    vector<double> tempVec;

    // off to the races. Record race results in local variables
    // vector<double> readRaceResults = ReadRace(vecCodes, listCodes, setCodes, FILE_NAME);
    // vector<double> sortRaceResults = SortRace(vecCodes, listCodes, setCodes);
    // vector<double> insertRaceResults = InsertRace(vecCodes, listCodes, setCodes, INSERT_CODE);
    // vector<double> deleteRaceResults = DeleteRace(vecCodes, listCodes, setCodes);

    // run each race 15 times and average results for each container type
    // create heats for each round of races
    // get time totals
    for (int i = 0; i < NUMBER_OF_HEATS; i++)
    {
        // get race results
        tempVec = ReadRace(vecCodes, listCodes, setCodes, FILE_NAME);
        transform(tempVec.begin(), tempVec.end(), vecResults.at(0).begin(), vecResults.at(0).begin(), [](double a, double b)
                  { return a + b; });

        tempVec = SortRace(vecCodes, listCodes, setCodes);
        transform(tempVec.begin(), tempVec.end(), vecResults.at(1).begin(), vecResults.at(1).begin(), [](double a, double b)
                  { return a + b; });

        tempVec = InsertRace(vecCodes, listCodes, setCodes, INSERT_CODE);
        transform(tempVec.begin(), tempVec.end(), vecResults.at(2).begin(), vecResults.at(2).begin(), [](double a, double b)
                  { return a + b; });

        tempVec = DeleteRace(vecCodes, listCodes, setCodes);
        transform(tempVec.begin(), tempVec.end(), vecResults.at(3).begin(), vecResults.at(3).begin(), [](double a, double b)
                  { return a + b; });
    }
    // average race times
    for (int i = 0; i < vecResults.size(); i++)
    {
        transform(vecResults.at(i).begin(), vecResults.at(i).end(), vecResults.at(i).begin(), [](double n)
                  { return n / NUMBER_OF_HEATS; });
    }

    // output results
    printf("After %2d heats, average scores are:", NUMBER_OF_HEATS);
    printf("%10s\t%10s\t%10s\t%10s\t\n", "Operation", "Vector (ms)", "List (ms)", "Set (ms)");
    printf("%10s\t%10f\t%10f\t%10f\t\n", "Read", vecResults.at(0).at(0), vecResults.at(0).at(1), vecResults.at(0).at(2));
    printf("%10s\t%10f\t%10f\t%10f\t\n", "Sort", vecResults.at(1).at(0), vecResults.at(1).at(1), vecResults.at(1).at(2));
    printf("%10s\t%10f\t%10f\t%10f\t\n", "Insert", vecResults.at(2).at(0), vecResults.at(2).at(1), vecResults.at(2).at(2));
    printf("%10s\t%10f\t%10f\t%10f\t\n", "Delete", vecResults.at(3).at(0), vecResults.at(3).at(1), vecResults.at(3).at(2));

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

    results.push_back(0); // set is already sorted
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
#pragma once

#ifndef SORTSTATE_H
#define SORTSTATE_H

#include <vector>
#include <chrono>
#include <string>

// Define the SortState structure
struct SortState {
    std::vector<int> array; // The array being sorted
    std::chrono::steady_clock::time_point startTime; // Start time of sorting
    float lastElapsed = 0.0f;
    int comparisons = 0;
    int swaps = 0;
    bool isSorting = false;

    int size = 10000;
    int minVal = 0;
    int maxVal = size;
};

extern SortState sortState;

// Function declarations
void initializeArray(int size, int minVal, int maxVal);
void initializeSort(int algorithmIndex);
void stopSort();
void resetSortState();
void updateSort(int algorithmIndex);
std::string getArrayState();
float getElapsedTime();
int getComparisons();
int getSwaps();

#endif // SORTSTATE_H
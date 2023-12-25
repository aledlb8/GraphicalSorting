#pragma once

#ifndef SORTSTATE_H
#define SORTSTATE_H

#include <vector>
#include <chrono>
#include <string>

struct SortState {
    std::vector<int> array;
    std::chrono::steady_clock::time_point startTime;
    float lastElapsed = 0.0f;
    int comparisons = 0;
    int swaps = 0;
    bool isSorting = false;

    int size = 10000;
    int minVal = 0;
    int maxVal = size;
};

extern SortState sortState;

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
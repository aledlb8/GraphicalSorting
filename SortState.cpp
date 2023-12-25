#include <vector>
#include <chrono>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>

#include "SortState.h"
#include "Sorting.h"

SortState sortState;

void initializeArray(int size, int minVal, int maxVal) {
    sortState.array.clear();
    sortState.array.reserve(size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(minVal, maxVal);

    for (int i = 0; i < size; ++i) {
        sortState.array.push_back(distr(gen));
    }
}

void initializeSort(int algorithmIndex) {
    initializeArray(sortState.size, sortState.minVal, sortState.maxVal);
    sortState.startTime = std::chrono::steady_clock::now();
    sortState.comparisons = 0;
    sortState.swaps = 0;
    sortState.isSorting = true;

    switch (algorithmIndex) {
    case 0:
        break;
    case 1:
        quickSortStack.push({ 0, static_cast<int>(sortState.array.size() - 1) });
        break;
    case 2:
        break;
    default:
        sortState.isSorting = false;
        break;
    }
}


void stopSort() {
    if (sortState.isSorting) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - sortState.startTime;
        sortState.lastElapsed = elapsed.count();
        sortState.isSorting = false;
    }
}

void resetSortState() {
    sortState.array.clear();
    sortState.isSorting = false;
    sortState.comparisons = 0;
    sortState.swaps = 0;
    sortState.startTime = std::chrono::steady_clock::now();
    sortState.lastElapsed = 0.0f;
}

void updateSort(int algorithmIndex) {
    if (!sortState.isSorting) {
        return;
    }

    switch (algorithmIndex) {
    case 0:
        updateBubbleSort();
        break;
    case 1:
        updateQuickSort();
        break;
    case 2:
        updateMergeSort();
        break;
    default:
        sortState.isSorting = false;
        break;
    }
}

// Get a string representation of the current array stat
std::string getArrayState() {
    std::string state = "[";
    for (size_t i = 0; i < sortState.array.size(); ++i) {
        if (i > 0) state += ", ";
        state += std::to_string(sortState.array[i]);
    }
    state += "]";
    return state;
}

float getElapsedTime() {
    using namespace std::chrono;

    if (sortState.isSorting) {
        auto now = steady_clock::now();
        duration<float> elapsed = now - sortState.startTime;
        return elapsed.count();
    }
    else {
        return sortState.lastElapsed;
    }
}

int getComparisons() {
    return sortState.comparisons;
}

int getSwaps() {
    return sortState.swaps;
}

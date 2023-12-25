#include <random>
#include <algorithm>

#include "Sorting.h"
#include "SortState.h"

std::stack<QuickSortState> quickSortStack;

int currentSize = 1;
int leftStart = 0;

void updateBubbleSort() {
    if (bubbleSortIndex >= sortState.array.size() - 1) {
        sortState.isSorting = false;
        return;
    }

    bool swapped = false;
    for (size_t i = 0; i < sortState.array.size() - bubbleSortIndex - 1; i++) {
        sortState.comparisons++;
        if (sortState.array[i] > sortState.array[i + 1]) {
            std::swap(sortState.array[i], sortState.array[i + 1]);
            sortState.swaps++;
            swapped = true;
        }
    }

    if (!swapped) {
        sortState.isSorting = false;
    }

    bubbleSortIndex++;
}



void updateQuickSort() {
    if (!sortState.isSorting) {
        return;
    }

    performQuickSortPartition();
}

void updateMergeSort() {
    if (!sortState.isSorting) {
        return;
    }

    int n = sortState.array.size();

    if (leftStart >= n - 1) {
        if (currentSize > n - 1) {
            sortState.isSorting = false;
            return;
        }

        currentSize = 2 * currentSize;
        leftStart = 0;
    }

    int mid = std::min(leftStart + currentSize - 1, n - 1);
    int right = std::min(leftStart + 2 * currentSize - 1, n - 1);

    performMerge(leftStart, mid, right);
    leftStart += 2 * currentSize;
}

int partition(int low, int high) {
    int pivot = sortState.array[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        sortState.comparisons++;
        if (sortState.array[j] <= pivot) {
            i++;
            std::swap(sortState.array[i], sortState.array[j]);
            sortState.swaps++;
        }
    }
    std::swap(sortState.array[i + 1], sortState.array[high]);
    sortState.swaps++;
    return (i + 1);
}


void performQuickSortPartition() {
    if (quickSortStack.empty()) {
        sortState.isSorting = false;
        return;
    }

    QuickSortState current = quickSortStack.top();
    quickSortStack.pop();

    if (current.low < current.high) {
        int pivot = partition(current.low, current.high);
        quickSortStack.push({ current.low, pivot - 1 });
        quickSortStack.push({ pivot + 1, current.high });
    }
}

void performMerge(int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // create temporary arrays
    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) {
        L[i] = sortState.array[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = sortState.array[mid + 1 + j];
    }

    // merge the temporary arrays back into the original array
    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        sortState.comparisons++;
        if (L[i] <= R[j]) {
            sortState.array[k] = L[i];
            i++;
        }
        else {
            sortState.array[k] = R[j];
            j++;
            sortState.swaps++;
        }
        k++;
    }

    while (i < n1) {
        sortState.array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        sortState.array[k] = R[j];
        j++;
        k++;
    }
}

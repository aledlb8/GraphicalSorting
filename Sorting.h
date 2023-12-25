#pragma once
#include <stack>

static int bubbleSortIndex = 0;

struct QuickSortState {
    int low;
    int high;
};

extern std::stack<QuickSortState> quickSortStack;;

void updateBubbleSort();
void updateQuickSort();
void updateMergeSort();

int partition(int low, int high);
void performQuickSortPartition();
void performMerge(int left, int mid, int right);
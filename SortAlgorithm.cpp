#include "SortAlgorithm.h"
#include "BubbleSort.h"
#include "QuickSort.h"
#include "MergeSort.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<SortAlgorithm> createSortAlgorithm(const std::string &algorithmName)
{
    if (algorithmName == "Bubble Sort")
    {
        return std::make_unique<BubbleSort>();
    }
    else if (algorithmName == "Quick Sort")
    {
        return std::make_unique<QuickSort>();
    }
    else if (algorithmName == "Merge Sort")
    {
        return std::make_unique<MergeSort>();
    }
    else
    {
        throw std::invalid_argument("Unknown sort algorithm: " + algorithmName);
    }
}
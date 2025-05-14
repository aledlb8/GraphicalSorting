#pragma once

#include "SortAlgorithm.h"
#include <vector>

class BubbleSort : public SortAlgorithm
{
public:
    BubbleSort() : SortAlgorithm("Bubble Sort"), bubbleSortIndex_(0) {}

    void initialize(std::vector<int> &array) override
    {
        array_ = &array; // Store a reference to the original array
        bubbleSortIndex_ = 0;
        metrics_.resetMetrics();
        metrics_.isRunning = true;
        metrics_.startTimer();
    }

    bool update() override
    {
        if (!metrics_.isRunning || bubbleSortIndex_ >= array_->size() - 1)
        {
            metrics_.isRunning = false;
            return false;
        }

        bool swapped = false;
        for (size_t i = 0; i < array_->size() - bubbleSortIndex_ - 1; i++)
        {
            metrics_.comparisons++;
            if ((*array_)[i] > (*array_)[i + 1])
            {
                std::swap((*array_)[i], (*array_)[i + 1]);
                metrics_.swaps++;
                swapped = true;
            }
        }

        if (!swapped)
        {
            metrics_.isRunning = false;
            return false;
        }

        bubbleSortIndex_++;
        return true;
    }

    const std::vector<int> &getCurrentArray() const override
    {
        return *array_;
    }

private:
    std::vector<int> *array_; // Pointer to the original array
    int bubbleSortIndex_;
};
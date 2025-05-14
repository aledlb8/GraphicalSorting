#pragma once

#include "SortAlgorithm.h"
#include <vector>
#include <stack>

class QuickSort : public SortAlgorithm
{
public:
    QuickSort() : SortAlgorithm("Quick Sort") {}

    void initialize(std::vector<int> &array) override
    {
        array_ = &array; // Store a reference to the original array
        sortStack_ = std::stack<std::pair<int, int>>();

        if (!array_->empty())
        {
            sortStack_.push({0, static_cast<int>(array_->size() - 1)});
        }

        metrics_.resetMetrics();
        metrics_.isRunning = true;
        metrics_.startTimer();
    }

    bool update() override
    {
        if (!metrics_.isRunning || sortStack_.empty())
        {
            metrics_.isRunning = false;
            return false;
        }

        // Get the next partition to process
        auto &top = sortStack_.top();
        int low = top.first;
        int high = top.second;
        sortStack_.pop();

        if (low < high)
        {
            // Perform partitioning
            int pivot = partition(low, high);

            // Push sub-partitions onto the stack
            sortStack_.push({low, pivot - 1});
            sortStack_.push({pivot + 1, high});
        }

        return !sortStack_.empty();
    }

    const std::vector<int> &getCurrentArray() const override
    {
        return *array_;
    }

private:
    int partition(int low, int high)
    {
        int pivot = (*array_)[high];
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++)
        {
            metrics_.comparisons++;
            if ((*array_)[j] <= pivot)
            {
                i++;
                std::swap((*array_)[i], (*array_)[j]);
                metrics_.swaps++;
            }
        }
        std::swap((*array_)[i + 1], (*array_)[high]);
        metrics_.swaps++;
        return (i + 1);
    }

    std::vector<int> *array_; // Pointer to the original array
    std::stack<std::pair<int, int>> sortStack_;
};
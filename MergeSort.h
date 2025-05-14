#pragma once

#include "SortAlgorithm.h"
#include <vector>
#include <algorithm>

class MergeSort : public SortAlgorithm
{
public:
    MergeSort() : SortAlgorithm("Merge Sort"), currentSize_(1), leftStart_(0) {}

    void initialize(std::vector<int> &array) override
    {
        array_ = &array; // Store a reference to the original array
        currentSize_ = 1;
        leftStart_ = 0;
        metrics_.resetMetrics();
        metrics_.isRunning = true;
        metrics_.startTimer();
    }

    bool update() override
    {
        if (!metrics_.isRunning)
        {
            return false;
        }

        int n = array_->size();

        if (leftStart_ >= n - 1)
        {
            if (currentSize_ > n - 1)
            {
                metrics_.isRunning = false;
                return false;
            }

            currentSize_ = 2 * currentSize_;
            leftStart_ = 0;
        }

        int mid = std::min(leftStart_ + currentSize_ - 1, n - 1);
        int right = std::min(leftStart_ + 2 * currentSize_ - 1, n - 1);

        performMerge(leftStart_, mid, right);
        leftStart_ += 2 * currentSize_;

        return true;
    }

    const std::vector<int> &getCurrentArray() const override
    {
        return *array_;
    }

private:
    void performMerge(int left, int mid, int right)
    {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        // Create temporary arrays
        std::vector<int> L(n1), R(n2);

        for (int i = 0; i < n1; i++)
        {
            L[i] = (*array_)[left + i];
        }
        for (int j = 0; j < n2; j++)
        {
            R[j] = (*array_)[mid + 1 + j];
        }

        // Merge the temporary arrays back into the original array
        int i = 0;
        int j = 0;
        int k = left;

        while (i < n1 && j < n2)
        {
            metrics_.comparisons++;
            if (L[i] <= R[j])
            {
                (*array_)[k] = L[i];
                i++;
            }
            else
            {
                (*array_)[k] = R[j];
                j++;
                metrics_.swaps++;
            }
            k++;
        }

        while (i < n1)
        {
            (*array_)[k] = L[i];
            i++;
            k++;
        }

        while (j < n2)
        {
            (*array_)[k] = R[j];
            j++;
            k++;
        }
    }

    std::vector<int> *array_; // Pointer to the original array
    int currentSize_;
    int leftStart_;
};
#pragma once

#include <vector>
#include <string>
#include <random>
#include <algorithm>

class SortArray
{
public:
    SortArray(int size = 10000, int minVal = 0, int maxVal = 10000)
        : size_(size), minVal_(minVal), maxVal_(maxVal)
    {
        generateRandomArray();
    }

    // Generate a new random array
    void generateRandomArray()
    {
        array_.clear();
        array_.reserve(size_);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(minVal_, maxVal_);

        for (int i = 0; i < size_; ++i)
        {
            array_.push_back(distr(gen));
        }
    }

    // Get array as string for display
    std::string toString() const
    {
        std::string state = "[";
        for (size_t i = 0; i < array_.size(); ++i)
        {
            if (i > 0)
                state += ", ";
            state += std::to_string(array_[i]);
        }
        state += "]";
        return state;
    }

    // Getters and setters
    const std::vector<int> &getArray() const { return array_; }
    std::vector<int> &getArray() { return array_; }

    int getSize() const { return size_; }
    void setSize(int size)
    {
        size_ = size;
        if (maxVal_ < size_)
        {
            maxVal_ = size_;
        }
    }

    int getMinVal() const { return minVal_; }
    void setMinVal(int val) { minVal_ = val; }

    int getMaxVal() const { return maxVal_; }
    void setMaxVal(int val) { maxVal_ = val; }

private:
    std::vector<int> array_;
    int size_;
    int minVal_;
    int maxVal_;
};
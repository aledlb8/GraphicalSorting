#pragma once

#include "SortAlgorithm.h"
#include "SortArray.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

class SortManager
{
public:
    SortManager();
    ~SortManager() = default;

    // Register available sorting algorithms
    void registerAlgorithms();

    // Get list of available algorithm names
    std::vector<std::string> getAvailableAlgorithmNames() const;

    // Get current selected algorithm name
    const std::string &getCurrentAlgorithmName() const;

    // Set algorithm by name
    bool setAlgorithm(const std::string &name);

    // Set algorithm by index
    bool setAlgorithm(int index);

    // Initialize sorting
    void initializeSort();

    // Update the sort process
    bool updateSort();

    // Stop sorting
    void stopSort();

    // Reset sorting state
    void resetSort();

    // Check if sorting is in progress
    bool isSorting() const;

    // Get metrics from the current algorithm
    const SortMetrics &getMetrics() const;

    // Get array state as string
    std::string getArrayAsString() const;

    // Access to the sort array
    SortArray &getSortArray() { return array_; }
    const SortArray &getSortArray() const { return array_; }

private:
    SortArray array_;
    std::vector<std::string> algorithmNames_;
    std::unordered_map<std::string, std::unique_ptr<SortAlgorithm>> algorithms_;
    SortAlgorithm *currentAlgorithm_ = nullptr;
    std::string currentAlgorithmName_;
};
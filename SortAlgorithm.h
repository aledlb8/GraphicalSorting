#pragma once

#include <vector>
#include <string>
#include <chrono>
#include <memory>

class SortMetrics
{
public:
    SortMetrics() : comparisons(0), swaps(0), lastElapsed(0.0f) {}

    void resetMetrics()
    {
        comparisons = 0;
        swaps = 0;
        lastElapsed = 0.0f;
        startTime = std::chrono::steady_clock::now();
    }

    float getElapsedTime() const
    {
        using namespace std::chrono;
        if (isRunning)
        {
            auto now = steady_clock::now();
            duration<float> elapsed = now - startTime;
            return elapsed.count();
        }
        return lastElapsed;
    }

    void stopTimer()
    {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed = now - startTime;
        lastElapsed = elapsed.count();
    }

    void startTimer()
    {
        startTime = std::chrono::steady_clock::now();
    }

    int comparisons;
    int swaps;
    bool isRunning = false;

private:
    std::chrono::steady_clock::time_point startTime;
    float lastElapsed;
};

class SortAlgorithm
{
public:
    SortAlgorithm(const std::string &name) : name_(name) {}
    virtual ~SortAlgorithm() = default;

    // Initialize algorithm-specific state
    virtual void initialize(std::vector<int> &array) = 0;

    // Perform one iteration of the sort algorithm
    virtual bool update() = 0;

    // Get the current array state
    virtual const std::vector<int> &getCurrentArray() const = 0;

    // Stop the sorting algorithm
    virtual void stop()
    {
        metrics_.isRunning = false;
        metrics_.stopTimer();
    }

    // Reset the algorithm state
    virtual void reset()
    {
        metrics_.resetMetrics();
    }

    // Return algorithm name
    const std::string &getName() const { return name_; }

    // Access to the sorting metrics
    SortMetrics &getMetrics() { return metrics_; }
    const SortMetrics &getMetrics() const { return metrics_; }

protected:
    std::string name_;
    SortMetrics metrics_;
};

// Factory method to create sort algorithms
std::unique_ptr<SortAlgorithm> createSortAlgorithm(const std::string &algorithmName);
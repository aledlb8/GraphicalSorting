#include "SortManager.h"
#include "SortAlgorithm.h"
#include <stdexcept>

SortManager::SortManager()
{
    registerAlgorithms();
    // Default to first algorithm
    if (!algorithmNames_.empty())
    {
        setAlgorithm(algorithmNames_[0]);
    }
}

void SortManager::registerAlgorithms()
{
    // Register available algorithms
    algorithms_.clear();
    algorithmNames_.clear();

    // Add algorithm names in desired order
    algorithmNames_.push_back("Bubble Sort");
    algorithmNames_.push_back("Quick Sort");
    algorithmNames_.push_back("Merge Sort");

    // Create algorithm instances
    for (const auto &name : algorithmNames_)
    {
        algorithms_[name] = createSortAlgorithm(name);
    }
}

std::vector<std::string> SortManager::getAvailableAlgorithmNames() const
{
    return algorithmNames_;
}

const std::string &SortManager::getCurrentAlgorithmName() const
{
    return currentAlgorithmName_;
}

bool SortManager::setAlgorithm(const std::string &name)
{
    auto it = algorithms_.find(name);
    if (it != algorithms_.end())
    {
        currentAlgorithm_ = it->second.get();
        currentAlgorithmName_ = name;
        return true;
    }
    return false;
}

bool SortManager::setAlgorithm(int index)
{
    if (index >= 0 && index < static_cast<int>(algorithmNames_.size()))
    {
        return setAlgorithm(algorithmNames_[index]);
    }
    return false;
}

void SortManager::initializeSort()
{
    if (currentAlgorithm_)
    {
        currentAlgorithm_->initialize(array_.getArray());
    }
}

bool SortManager::updateSort()
{
    if (currentAlgorithm_ && currentAlgorithm_->getMetrics().isRunning)
    {
        bool continueSort = currentAlgorithm_->update();
        
        return continueSort;
    }
    return false;
}

void SortManager::stopSort()
{
    if (currentAlgorithm_)
    {
        currentAlgorithm_->stop();
    }
}

void SortManager::resetSort()
{
    if (currentAlgorithm_)
    {
        currentAlgorithm_->reset();
    }

    // Generate a new random array
    array_.generateRandomArray();
}

bool SortManager::isSorting() const
{
    return currentAlgorithm_ && currentAlgorithm_->getMetrics().isRunning;
}

const SortMetrics &SortManager::getMetrics() const
{
    static SortMetrics emptyMetrics;
    if (currentAlgorithm_)
    {
        return currentAlgorithm_->getMetrics();
    }
    return emptyMetrics;
}

std::string SortManager::getArrayAsString() const
{
    return array_.toString();
}
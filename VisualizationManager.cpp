#include "VisualizationManager.h"
#include "BarGraphVisualization.h"
#include "HeatmapVisualization.h"
#include "ParticleSystemVisualization.h"

VisualizationManager::VisualizationManager(SortManager &sortManager)
    : sortManager_(sortManager)
{
    registerVisualizations();
}

void VisualizationManager::registerVisualizations()
{
    // Clear existing visualizations
    visualizations_.clear();
    visualizationNames_.clear();
    activeVisualizations_.clear();

    // Add new visualizations
    auto barGraph = std::make_unique<BarGraphVisualization>();
    auto heatmap = std::make_unique<HeatmapVisualization>();
    auto particleSystem = std::make_unique<ParticleSystemVisualization>();

    // Store visualization names
    std::string barGraphName = barGraph->getName();
    std::string heatmapName = heatmap->getName();
    std::string particleSystemName = particleSystem->getName();

    // Add to name list (defines the order)
    visualizationNames_.push_back(barGraphName);
    visualizationNames_.push_back(heatmapName);
    visualizationNames_.push_back(particleSystemName);

    // Add to map of visualizations
    visualizations_[barGraphName] = std::move(barGraph);
    visualizations_[heatmapName] = std::move(heatmap);
    visualizations_[particleSystemName] = std::move(particleSystem);

    // Initialize active state (all disabled by default)
    for (const auto &name : visualizationNames_)
    {
        activeVisualizations_[name] = false;
    }
}

void VisualizationManager::renderVisualizations()
{
    for (const auto &name : visualizationNames_)
    {
        auto it = activeVisualizations_.find(name);
        if (it != activeVisualizations_.end() && it->second)
        {
            auto visIt = visualizations_.find(name);
            if (visIt != visualizations_.end())
            {
                visIt->second->render(sortManager_, it->second);
                // Update the active state if the window was closed
                activeVisualizations_[name] = it->second;
            }
        }
    }
}

bool VisualizationManager::isVisualizationEnabled(const std::string &name) const
{
    auto it = activeVisualizations_.find(name);
    return it != activeVisualizations_.end() ? it->second : false;
}

void VisualizationManager::setVisualizationEnabled(const std::string &name, bool enabled)
{
    activeVisualizations_[name] = enabled;
}

void VisualizationManager::toggleVisualization(const std::string &name)
{
    auto it = activeVisualizations_.find(name);
    if (it != activeVisualizations_.end())
    {
        it->second = !it->second;
    }
}

std::vector<std::string> VisualizationManager::getVisualizationNames() const
{
    return visualizationNames_;
}

const std::unordered_map<std::string, std::unique_ptr<IVisualization>>& VisualizationManager::getVisualizations() const
{
    return visualizations_;
}
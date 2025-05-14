#pragma once

#include "IVisualization.h"
#include "SortManager.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

class VisualizationManager
{
public:
    VisualizationManager(SortManager &sortManager);
    ~VisualizationManager() = default;

    // Register available visualizations
    void registerVisualizations();

    // Render all active visualizations
    void renderVisualizations();

    // Get state of visualization by name
    bool isVisualizationEnabled(const std::string &name) const;

    // Set state of visualization by name
    void setVisualizationEnabled(const std::string &name, bool enabled);

    // Toggle visualization state
    void toggleVisualization(const std::string &name);

    // Get all available visualization names
    std::vector<std::string> getVisualizationNames() const;
    
    // Get access to visualizations map
    const std::unordered_map<std::string, std::unique_ptr<IVisualization>>& getVisualizations() const;

private:
    SortManager &sortManager_;
    std::vector<std::string> visualizationNames_;
    std::unordered_map<std::string, std::unique_ptr<IVisualization>> visualizations_;
    std::unordered_map<std::string, bool> activeVisualizations_;
};
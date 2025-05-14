#pragma once

#include "SortManager.h"
#include "VisualizationManager.h"
#include "lib/ImGui/imgui.h"

class Application
{
public:
    Application();
    ~Application() = default;

    // Main render method called each frame
    void render();

private:
    // Render the main control window
    void renderMainWindow();

    // Render the control panel
    void renderControlPanel();
    
    // Setup ImGui styling
    void setupStyle();
    
    // Render the status bar
    void renderStatusBar();
    
    // Render active visualizations
    void renderVisualizations(float startX, float startY, float availWidth, float availHeight);
    
    // Check if array is fully sorted
    bool isArrayFullySorted() const;
    
    // Get algorithm information
    std::string getAlgorithmComplexity(const std::string& algorithmName) const;
    std::string getAlgorithmDescription(const std::string& algorithmName) const;
    
    // Get visualization description
    std::string getVisualizationDescription(const std::string& visName) const;

    SortManager sortManager_;
    VisualizationManager visualizationManager_;

    // UI state
    bool showControlPanel_;
};
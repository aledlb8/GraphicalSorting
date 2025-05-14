#pragma once

#include "SortManager.h"
#include "lib/ImGui/imgui.h"

class IVisualization
{
public:
    virtual ~IVisualization() = default;

    // Handle the ImGui rendering of the visualization
    virtual void render(const SortManager &sortManager, bool &isVisible) = 0;

    // Get the name of the visualization
    virtual const char *getName() const = 0;
};
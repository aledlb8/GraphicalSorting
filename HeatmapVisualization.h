#pragma once

#include "IVisualization.h"

class HeatmapVisualization : public IVisualization
{
public:
    HeatmapVisualization() = default;
    ~HeatmapVisualization() override = default;

    void render(const SortManager &sortManager, bool &isVisible) override;
    const char *getName() const override { return "Heatmap"; }
};
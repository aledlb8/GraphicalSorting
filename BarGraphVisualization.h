#pragma once

#include "IVisualization.h"

class BarGraphVisualization : public IVisualization
{
public:
    BarGraphVisualization() = default;
    ~BarGraphVisualization() override = default;

    void render(const SortManager &sortManager, bool &isVisible) override;
    const char *getName() const override { return "Bar Graph"; }
};
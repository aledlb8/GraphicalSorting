#pragma once

#include "IVisualization.h"
#include <vector>

struct Particle
{
    float x, y;     // Position
    float vx, vy;   // Velocity
    float lifetime; // Lifetime in seconds
    float size;     // Size of the particle
    float r, g, b;  // Color
};

class ParticleSystemVisualization : public IVisualization
{
public:
    ParticleSystemVisualization();
    ~ParticleSystemVisualization() override = default;

    void render(const SortManager &sortManager, bool &isVisible) override;
    const char *getName() const override { return "Particle System"; }

private:
    void updateParticles(float deltaTime);
    void spawnParticles(const SortManager &sortManager);

    std::vector<Particle> particles_;
    float lastUpdateTime_;
    int maxParticles_;
};
#include "ParticleSystemVisualization.h"
#include "lib/ImGui/imgui.h"
#include <random>
#include <algorithm>
#include <chrono>

ParticleSystemVisualization::ParticleSystemVisualization()
    : lastUpdateTime_(0.0f), maxParticles_(5000)
{
    particles_.reserve(maxParticles_);
}

void ParticleSystemVisualization::render(const SortManager &sortManager, bool &isVisible)
{
    // Window management is now handled by the Application class

    // Add debug info
    ImGui::Text("Particles: %d/%d", static_cast<int>(particles_.size()), maxParticles_);
    ImGui::Text("Sorting in progress: %s", sortManager.isSorting() ? "Yes" : "No");

    // Get current time
    float currentTime = ImGui::GetTime();
    float deltaTime = lastUpdateTime_ == 0.0f ? 0.016f : currentTime - lastUpdateTime_;
    lastUpdateTime_ = currentTime;

    // Cap deltaTime to avoid large jumps
    deltaTime = std::min(deltaTime, 0.1f);

    // Update existing particles
    updateParticles(deltaTime);

    // Always spawn particles, more when sorting
    spawnParticles(sortManager);

    // Get the full available content region for drawing
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();

    // Set minimum canvas size to ensure visibility
    if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
    if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;

    // Create an invisible button that covers the canvas area to capture mouse interactions
    ImGui::InvisibleButton("##particle_canvas", canvas_size, ImGuiButtonFlags_MouseButtonLeft);
    
    // Highlight if it's hovered
    bool isHovered = ImGui::IsItemHovered();
    
    // Get window draw list for rendering
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    
    // Draw background for the canvas with a gradient
    ImColor bgTop(20, 20, 40, 255);    // Dark blue-ish top
    ImColor bgBottom(10, 10, 20, 255); // Even darker bottom
    
    draw_list->AddRectFilledMultiColor(
        canvas_pos,
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
        bgTop, bgTop, bgBottom, bgBottom
    );

    // Draw each particle
    if (!particles_.empty())
    {
        for (const auto &particle : particles_)
        {
            // Calculate particle position based on canvas size and particle's relative position
            ImVec2 pos(
                canvas_pos.x + particle.x * canvas_size.x,
                canvas_pos.y + particle.y * canvas_size.y);

            // Scale particle size based on canvas dimensions
            float baseSize = std::min(canvas_size.x, canvas_size.y) / 100.0f;
            float radius = particle.size * baseSize * 10.0f; // Increased size for visibility
            
            // Create a glow effect with alpha falloff
            ImU32 innerColor = IM_COL32(
                static_cast<int>(particle.r * 255),
                static_cast<int>(particle.g * 255),
                static_cast<int>(particle.b * 255),
                200);
                
            ImU32 outerColor = IM_COL32(
                static_cast<int>(particle.r * 255),
                static_cast<int>(particle.g * 255),
                static_cast<int>(particle.b * 255),
                50);
                
            // Draw outer glow
            draw_list->AddCircleFilled(pos, radius * 1.5f, outerColor);
            
            // Draw inner particle
            draw_list->AddCircleFilled(pos, radius, innerColor);
        }
    }
    else
    {
        // Center the text if no particles
        float textWidth = ImGui::CalcTextSize("No particles - will generate soon").x;
        ImGui::SetCursorPosX(canvas_pos.x + (canvas_size.x - textWidth) * 0.5f);
        ImGui::SetCursorPosY(canvas_pos.y + canvas_size.y * 0.5f);
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No particles - will generate soon");
    }
    
    // Add border around the canvas
    ImU32 borderColor = isHovered ? IM_COL32(150, 150, 150, 255) : IM_COL32(80, 80, 80, 255);
    draw_list->AddRect(
        canvas_pos,
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
        borderColor, 0.0f, 0, 1.5f // Slightly thicker border
    );
}

void ParticleSystemVisualization::updateParticles(float deltaTime)
{
    // Skip updating if deltaTime is invalid
    if (deltaTime <= 0.0f)
        return;

    // Update all particles and remove dead ones
    auto it = particles_.begin();
    while (it != particles_.end())
    {
        // Update position
        it->x += it->vx * deltaTime;
        it->y += it->vy * deltaTime;

        // Apply gravity
        it->vy += 0.4f * deltaTime;

        // Reduce lifetime
        it->lifetime -= deltaTime;

        // Remove if expired or out of bounds
        if (it->lifetime <= 0.0f || it->y > 1.1f || it->x < -0.1f || it->x > 1.1f)
        {
            it = particles_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void ParticleSystemVisualization::spawnParticles(const SortManager &sortManager)
{
    // Random number generation
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist_size(0.01f, 0.03f);
    static std::uniform_real_distribution<float> dist_lifetime(0.5f, 3.0f);
    static std::uniform_real_distribution<float> dist_vel(-0.3f, 0.3f);

    // Don't spawn too many particles
    if (particles_.size() >= maxParticles_)
        return;

    const auto &array = sortManager.getSortArray().getArray();
    const int arraySize = array.size();
    const int maxVal = sortManager.getSortArray().getMaxVal();

    // Only spawn if we have array elements
    if (arraySize == 0)
        return;

    // Spawn particles proportional to sorting activity
    int numToSpawn = sortManager.isSorting() ? 100 : 20;
    numToSpawn = std::min(numToSpawn, static_cast<int>(maxParticles_ - particles_.size()));

    for (int i = 0; i < numToSpawn; ++i)
    {
        // Pick a random index for a data point
        std::uniform_int_distribution<int> dist_idx(0, arraySize - 1);
        int idx = dist_idx(gen);

        // Create particle at position corresponding to array element
        Particle p;

        // Position based on array index and value
        p.x = static_cast<float>(idx) / arraySize;
        p.y = 1.0f - (static_cast<float>(array[idx]) / maxVal);

        // Random initial velocity with upward bias
        p.vx = dist_vel(gen);
        p.vy = dist_vel(gen) - 0.5f; // Bias upward more

        // Size and lifetime
        p.size = dist_size(gen);
        p.lifetime = dist_lifetime(gen);

        // Color based on array value (using the same heat map color scheme)
        float value = static_cast<float>(array[idx]) / maxVal;
        p.r = value > 0.5f ? 1.0f : 2.0f * value;
        p.g = value > 0.5f ? 2.0f * (1.0f - value) : 2.0f * value;
        p.b = value > 0.5f ? 0.0f : 1.0f - 2.0f * value;

        particles_.push_back(p);
    }
}
#include "HeatmapVisualization.h"
#include "lib/ImGui/imgui.h"

// Helper to interpolate colors
ImColor getHeatMapColor(float value)
{
    // Color interpolation from blue (cold, low values) to red (hot, high values)
    const float r = value > 0.5f ? 1.0f : 2.0f * value;
    const float g = value > 0.5f ? 2.0f * (1.0f - value) : 2.0f * value;
    const float b = value > 0.5f ? 0.0f : 1.0f - 2.0f * value;

    return ImColor(r, g, b);
}

void HeatmapVisualization::render(const SortManager &sortManager, bool &isVisible)
{
    // Window management is now handled by the Application class
    
    // Add title and info for the visualization
    ImGui::Text("Heatmap Visualization");
    
    if (sortManager.isSorting()) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Sorting in progress...");
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Paused");
    }
    
    // Get the full available content region for drawing
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();

    // Set minimum canvas size to ensure visibility
    if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
    if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;

    // Create an invisible button that covers the canvas area to capture mouse interactions
    ImGui::InvisibleButton("##heatmap_canvas", canvas_size, ImGuiButtonFlags_MouseButtonLeft);
    
    // Get window draw list for rendering
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    
    // Draw background for the canvas
    draw_list->AddRectFilled(
        canvas_pos,
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
        IM_COL32(20, 20, 20, 255) // Dark background
    );

    const auto &array = sortManager.getSortArray().getArray();
    const int arraySize = array.size();
    const int maxVal = sortManager.getSortArray().getMaxVal();

    // Don't try to render if array is empty
    if (arraySize > 0)
    {
        // Calculate cell size based on array size (limit to a reasonable number of cells)
        const int maxCellsToDisplay = std::min(arraySize, 500);
        const float cellWidth = canvas_size.x / maxCellsToDisplay;
        const float cellHeight = canvas_size.y / 50; // Fixed number of rows

        // Draw heatmap cells
        for (int i = 0; i < maxCellsToDisplay; ++i)
        {
            const float normalizedValue = array[i] / static_cast<float>(maxVal);

            ImVec2 p0(canvas_pos.x + i * cellWidth,
                      canvas_pos.y + (1.0f - normalizedValue) * canvas_size.y);
            ImVec2 p1(canvas_pos.x + (i + 1) * cellWidth,
                      canvas_pos.y + canvas_size.y);

            draw_list->AddRectFilled(
                p0, p1,
                getHeatMapColor(normalizedValue));
        }
    }
    
    // Add border around the canvas
    draw_list->AddRect(
        canvas_pos,
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
        IM_COL32(100, 100, 100, 255) // Gray border
    );
}
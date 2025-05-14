#include "BarGraphVisualization.h"
#include "lib/ImGui/imgui.h"

void BarGraphVisualization::render(const SortManager &sortManager, bool &isVisible)
{
    // Window management is now handled by the Application class
    
    // Calculate available space for visualization, accounting for the UI elements
    const float headerHeight = ImGui::GetTextLineHeightWithSpacing() * 4; // Approximate height of header text
    
    // Add debug info
    const auto &array = sortManager.getSortArray().getArray();
    ImGui::Text("Array size: %d", static_cast<int>(array.size()));
    
    if (sortManager.isSorting()) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Sorting in progress...");
    } else {
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Sorting paused/stopped");
    }
    
    // Display the first few values for debugging
    if (!array.empty()) {
        ImGui::Text("First values: %d, %d, %d...", 
                    array[0], 
                    array.size() > 1 ? array[1] : 0, 
                    array.size() > 2 ? array[2] : 0);
    }
    
    // Get the full available content region for drawing
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();
    
    // Set minimum canvas size to ensure visibility
    if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
    if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;

    // Create an invisible button that covers the canvas area to capture mouse interactions
    ImGui::InvisibleButton("##canvas", canvas_size, ImGuiButtonFlags_MouseButtonLeft);
    
    // Get window draw list for rendering
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    
    // Draw background for the canvas
    draw_list->AddRectFilled(
        canvas_pos,
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
        IM_COL32(30, 30, 30, 255) // Dark background
    );

    const int arraySize = array.size();
    const int maxVal = sortManager.getSortArray().getMaxVal();

    // Don't try to render if array is empty
    if (arraySize > 0)
    {
        // Calculate bar width based on available canvas size and array size
        const float barWidth = canvas_size.x / arraySize;
        
        // Draw the bars
        for (int i = 0; i < arraySize; ++i)
        {
            float barHeight = (array[i] / static_cast<float>(maxVal)) * canvas_size.y;

            // Calculate bar position 
            ImVec2 p0(canvas_pos.x + i * barWidth, canvas_pos.y + canvas_size.y - barHeight);
            ImVec2 p1(canvas_pos.x + (i + 1) * barWidth, canvas_pos.y + canvas_size.y);

            // Use different colors for sorted/unsorted parts
            ImU32 color = IM_COL32(255, 255, 255, 255);
            
            // Add some color variation
            if (sortManager.isSorting()) {
                // Highlight currently processed elements with a different color
                float normalizedValue = array[i] / static_cast<float>(maxVal);
                color = IM_COL32(
                    static_cast<int>(255 * normalizedValue),  // Red
                    static_cast<int>(255 * (1.0f - normalizedValue)),  // Green
                    100,  // Blue
                    255   // Alpha
                );
            }
            
            // Draw the bar
            draw_list->AddRectFilled(p0, p1, color);
        }
    }
    
    // Add border around the canvas
    draw_list->AddRect(
        canvas_pos,
        ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y),
        IM_COL32(100, 100, 100, 255) // Gray border
    );
}
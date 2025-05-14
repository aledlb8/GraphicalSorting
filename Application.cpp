#include "Application.h"
#include "lib/ImGui/imgui.h"
#include "lib/ImGui/imgui_internal.h"
#include <string>
#include <cmath>
#include <algorithm>

// Custom clamp function since std::clamp requires C++17
template <typename T>
T clamp(T value, T min, T max)
{
    return (value < min) ? min : ((value > max) ? max : value);
}

// Professional UI colors
const ImVec4 COLOR_PRIMARY = ImVec4(0.20f, 0.47f, 0.67f, 1.0f);   // Blue
const ImVec4 COLOR_SECONDARY = ImVec4(0.24f, 0.27f, 0.33f, 1.0f); // Dark gray
const ImVec4 COLOR_ACCENT = ImVec4(0.96f, 0.44f, 0.18f, 1.0f);    // Orange
const ImVec4 COLOR_TEXT = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);      // Light text
const ImVec4 COLOR_TEXT_DIM = ImVec4(0.70f, 0.70f, 0.70f, 1.0f);  // Dimmed text
const ImVec4 COLOR_BG_DARK = ImVec4(0.16f, 0.18f, 0.21f, 1.0f);   // Dark background
const ImVec4 COLOR_BG_PANEL = ImVec4(0.22f, 0.24f, 0.27f, 1.0f);  // Panel background
const ImVec4 COLOR_SUCCESS = ImVec4(0.34f, 0.74f, 0.39f, 1.0f);   // Green
const ImVec4 COLOR_WARNING = ImVec4(0.90f, 0.70f, 0.10f, 1.0f);   // Yellow

// Window layout sizes and positions
const float MAIN_WINDOW_WIDTH = 350.0f;
const float CONTROL_PANEL_WIDTH = 350.0f;
const float VISUAL_PANEL_WIDTH = 600.0f;
const float VISUAL_PANEL_HEIGHT = 450.0f;
const float MARGIN = 10.0f;
const float STATUS_BAR_HEIGHT = 24.0f;

Application::Application()
    : sortManager_(), visualizationManager_(sortManager_), showControlPanel_(true)
{
    setupStyle();

    // Enable visualizations by default
    for (const auto &name : visualizationManager_.getVisualizationNames())
    {
        visualizationManager_.setVisualizationEnabled(name, true);
    }
}

void Application::setupStyle()
{
    ImGuiStyle &style = ImGui::GetStyle();

    // Colors
    ImVec4 *colors = style.Colors;
    colors[ImGuiCol_Text] = COLOR_TEXT;
    colors[ImGuiCol_TextDisabled] = COLOR_TEXT_DIM;
    colors[ImGuiCol_WindowBg] = COLOR_BG_DARK;
    colors[ImGuiCol_ChildBg] = COLOR_BG_DARK;
    colors[ImGuiCol_PopupBg] = COLOR_BG_PANEL;
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.27f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.26f, 0.33f, 1.00f);
    colors[ImGuiCol_TitleBg] = COLOR_SECONDARY;
    colors[ImGuiCol_TitleBgActive] = COLOR_PRIMARY;
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.16f, 0.19f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.17f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.17f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.36f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.46f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.56f, 1.00f);
    colors[ImGuiCol_CheckMark] = COLOR_ACCENT;
    colors[ImGuiCol_SliderGrab] = COLOR_PRIMARY;
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = COLOR_PRIMARY;
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header] = COLOR_PRIMARY;
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = COLOR_SECONDARY;
    colors[ImGuiCol_TabHovered] = COLOR_PRIMARY;
    colors[ImGuiCol_TabActive] = COLOR_PRIMARY;
    colors[ImGuiCol_TabUnfocused] = COLOR_SECONDARY;
    colors[ImGuiCol_TabUnfocusedActive] = COLOR_SECONDARY;
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    // Style
    style.FrameRounding = 4.0f;
    style.FramePadding = ImVec2(8.0f, 6.0f);
    style.ItemSpacing = ImVec2(10.0f, 8.0f);
    style.WindowRounding = 6.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 6.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowPadding = ImVec2(10.0f, 10.0f);
    style.DisplaySafeAreaPadding = ImVec2(8.0f, 8.0f);
    style.DisplayWindowPadding = ImVec2(20.0f, 20.0f);
    style.AntiAliasedLines = true;
    style.AntiAliasedFill = true;
    style.CurveTessellationTol = 1.25f;
}

void Application::render()
{
    // Position main windows
    ImGuiIO &io = ImGui::GetIO();

    // Calculate layout based on screen size
    float startPosX = MARGIN;
    float startPosY = MARGIN;

    float availWidth = io.DisplaySize.x - (MARGIN * 2);
    float availHeight = io.DisplaySize.y - (MARGIN * 2);

    // Set main window position and size
    ImGui::SetNextWindowPos(ImVec2(startPosX, startPosY));
    ImGui::SetNextWindowSize(ImVec2(MAIN_WINDOW_WIDTH, availHeight / 2.0f));

    // Render main application window
    renderMainWindow();

    // Position control panel
    ImGui::SetNextWindowPos(ImVec2(startPosX, startPosY + availHeight / 2.0f + MARGIN));
    ImGui::SetNextWindowSize(ImVec2(MAIN_WINDOW_WIDTH, availHeight / 2.0f - MARGIN));

    // Render control panel
    renderControlPanel();

    // Set visualization panel positions (to the right of main windows)
    float visualPosX = startPosX + MAIN_WINDOW_WIDTH + MARGIN;
    float visualWidth = availWidth - MAIN_WINDOW_WIDTH - MARGIN;

    // Update sorting if in progress - do multiple iterations per frame for better visualization
    if (sortManager_.isSorting())
    {
        // Perform multiple iterations per frame for faster visual progress
        // The number of iterations depends on array size - larger arrays need more iterations
        int iterationsPerFrame = std::max(1, sortManager_.getSortArray().getSize() / 1000);

        // Limit to a reasonable number
        iterationsPerFrame = std::min(iterationsPerFrame, 50);

        for (int i = 0; i < iterationsPerFrame && sortManager_.isSorting(); i++)
        {
            sortManager_.updateSort();
        }
    }

    // Render status bar at the bottom of the screen
    renderStatusBar();

    // Render active visualizations with proper window positions and sizes
    renderVisualizations(visualPosX, startPosY, visualWidth, availHeight);
}

void Application::renderMainWindow()
{
    ImGui::Begin("Sorting Algorithm", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // Logo or title with larger font
    {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::TextColored(COLOR_ACCENT, "GraphSort Pro");
        ImGui::PopFont();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Algorithm Selection ComboBox
    {
        static int selectedAlgorithmIndex = 0;
        auto algorithmNames = sortManager_.getAvailableAlgorithmNames();

        ImGui::TextColored(COLOR_TEXT_DIM, "Algorithm:");

        if (ImGui::Combo("##Algorithm", &selectedAlgorithmIndex, [](void *data, int idx, const char **out_text)
                         {
				auto* names = static_cast<std::vector<std::string>*>(data);
				if (idx < 0 || idx >= static_cast<int>(names->size())) return false;
				*out_text = (*names)[idx].c_str();
				return true; }, &algorithmNames, algorithmNames.size()))
        {
            sortManager_.setAlgorithm(selectedAlgorithmIndex);
        }
    }

    ImGui::Spacing();

    // Array size slider
    {
        int size = sortManager_.getSortArray().getSize();
        ImGui::TextColored(COLOR_TEXT_DIM, "Array Size:");

        if (ImGui::SliderInt("##Array Size", &size, 100, 10000, "%d elements"))
        {
            sortManager_.getSortArray().setSize(size);
            sortManager_.resetSort(); // Reset with new size
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Control buttons
    {
        ImGui::PushStyleColor(ImGuiCol_Button, sortManager_.isSorting() ? COLOR_WARNING : COLOR_SUCCESS);

        // Start/Stop button with icon
        if (ImGui::Button(sortManager_.isSorting() ? "Stop Sorting" : "Start Sorting",
                          ImVec2(ImGui::GetContentRegionAvail().x, 40.0f)))
        {
            if (sortManager_.isSorting())
            {
                sortManager_.stopSort();
            }
            else
            {
                sortManager_.initializeSort();
            }
        }

        ImGui::PopStyleColor();

        ImGui::Spacing();

        // Reset button
        if (ImGui::Button("Reset Array", ImVec2(ImGui::GetContentRegionAvail().x, 30.0f)))
        {
            sortManager_.resetSort();
        }
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Visualization Panels
    ImGui::TextColored(COLOR_TEXT_DIM, "Visualization Options:");
    ImGui::Spacing();

    // Group the visualization checkboxes in a frame
    ImGui::BeginChild("VisualizationOptions", ImVec2(0, 120), true);

    for (const auto &visName : visualizationManager_.getVisualizationNames())
    {
        bool isEnabled = visualizationManager_.isVisualizationEnabled(visName);

        if (ImGui::Checkbox(visName.c_str(), &isEnabled))
        {
            visualizationManager_.setVisualizationEnabled(visName, isEnabled);
        }

        // Add description tooltips
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted(getVisualizationDescription(visName).c_str());
            ImGui::EndTooltip();
        }
    }

    ImGui::EndChild();

    ImGui::End();
}

void Application::renderControlPanel()
{
    ImGui::Begin("Performance Metrics", &showControlPanel_,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse);

    // Performance metrics section
    {
        // Get metrics data
        const auto &metrics = sortManager_.getMetrics();
        float elapsedTime = metrics.getElapsedTime();
        int comparisons = metrics.comparisons;
        int swaps = metrics.swaps;
        float fps = ImGui::GetIO().Framerate;

        // Progress indicator
        ImGui::TextColored(COLOR_TEXT_DIM, "Sort Progress:");

        // Calculate progress based on comparisons (very rough estimate)
        float progress = 0.0f;
        int arraySize = sortManager_.getSortArray().getSize();
        if (arraySize > 0)
        {
            // This is just a rough estimate - different algorithms have different complexity
            float totalExpectedComparisons = arraySize * std::log2(arraySize) * 0.5f;
            progress = std::min(1.0f, comparisons / std::max(1.0f, totalExpectedComparisons));
        }

        // Progress bar
        ImGui::ProgressBar(progress, ImVec2(-1, 15), sortManager_.isSorting() ? "Sorting..." : "Ready");

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // Statistics section
        ImGui::TextColored(COLOR_TEXT_DIM, "Statistics:");

        ImGui::BeginTable("MetricsTable", 2, ImGuiTableFlags_SizingStretchProp);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Time Elapsed:");
        ImGui::TableNextColumn();
        ImGui::TextColored(COLOR_PRIMARY, "%.3f sec", elapsedTime);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Comparisons:");
        ImGui::TableNextColumn();
        ImGui::TextColored(COLOR_PRIMARY, "%d", comparisons);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Swaps:");
        ImGui::TableNextColumn();
        ImGui::TextColored(COLOR_PRIMARY, "%d", swaps);

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Frames per Second:");
        ImGui::TableNextColumn();
        ImGui::TextColored(COLOR_PRIMARY, "%.1f FPS", fps);

        ImGui::EndTable();
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Algorithm complexity information
    {
        ImGui::TextColored(COLOR_TEXT_DIM, "Algorithm Complexity:");

        std::string algorithmName = sortManager_.getCurrentAlgorithmName();
        std::string complexity = getAlgorithmComplexity(algorithmName);
        std::string description = getAlgorithmDescription(algorithmName);

        ImGui::TextColored(COLOR_PRIMARY, "%s", complexity.c_str());
        ImGui::Spacing();
        ImGui::TextWrapped("%s", description.c_str());
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Array state preview
    {
        ImGui::TextColored(COLOR_TEXT_DIM, "Array Preview:");

        bool isArraySorted = isArrayFullySorted();

        if (isArraySorted)
        {
            ImGui::TextColored(COLOR_SUCCESS, "Array is sorted!");
        }
        else
        {
            ImGui::TextColored(COLOR_WARNING, "Array is not fully sorted");
        }

        const auto &array = sortManager_.getSortArray().getArray();

        // Simple array preview
        ImGui::BeginChild("ArrayPreview", ImVec2(0, 80), true, ImGuiWindowFlags_HorizontalScrollbar);

        if (!array.empty())
        {
            // Display the beginning of the array
            std::string begin = "Start: ";
            for (int i = 0; i < std::min(10, static_cast<int>(array.size())); i++)
            {
                begin += std::to_string(array[i]) + ", ";
            }

            // Also show some from the end if large enough
            if (array.size() > 20)
            {
                std::string end = "End: ";
                for (int i = std::max(10, static_cast<int>(array.size() - 10));
                     i < static_cast<int>(array.size()); i++)
                {
                    end += std::to_string(array[i]) + ", ";
                }

                ImGui::TextUnformatted(begin.c_str());
                ImGui::TextUnformatted(end.c_str());
            }
            else
            {
                ImGui::TextUnformatted(begin.c_str());
            }
        }
        else
        {
            ImGui::TextColored(COLOR_WARNING, "No array data available");
        }

        ImGui::EndChild();
    }

    ImGui::End();
}

void Application::renderStatusBar()
{
    // Create a status bar at the bottom of the screen
    ImGuiIO &io = ImGui::GetIO();
    ImVec2 statusBarPos = ImVec2(0, io.DisplaySize.y - STATUS_BAR_HEIGHT);
    ImVec2 statusBarSize = ImVec2(io.DisplaySize.x, STATUS_BAR_HEIGHT);

    ImGui::SetNextWindowPos(statusBarPos);
    ImGui::SetNextWindowSize(statusBarSize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 2));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.12f, 1.0f));

    ImGui::Begin("StatusBar", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);

    // Status message
    std::string statusText;
    ImVec4 statusColor;

    if (sortManager_.isSorting())
    {
        statusText = "Sorting in progress...";
        statusColor = COLOR_PRIMARY;
    }
    else if (isArrayFullySorted())
    {
        statusText = "Sort complete - array is sorted";
        statusColor = COLOR_SUCCESS;
    }
    else
    {
        statusText = "Ready";
        statusColor = COLOR_TEXT;
    }

    ImGui::TextColored(statusColor, "%s", statusText.c_str());

    // Right-aligned items (array size, algorithm name)
    float statusBarWidth = ImGui::GetWindowWidth();

    const char *algorithmName = sortManager_.getCurrentAlgorithmName().c_str();
    int arraySize = sortManager_.getSortArray().getSize();

    std::string rightText = "Algorithm: " + std::string(algorithmName) +
                            " | Array Size: " + std::to_string(arraySize);

    float rightTextWidth = ImGui::CalcTextSize(rightText.c_str()).x;

    ImGui::SameLine(statusBarWidth - rightTextWidth - 10);
    ImGui::TextColored(COLOR_TEXT_DIM, "%s", rightText.c_str());

    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}

void Application::renderVisualizations(float startX, float startY,
                                       float availWidth, float availHeight)
{
    // Get active visualizations
    const auto &visNames = visualizationManager_.getVisualizationNames();
    std::vector<std::string> activeVisualizations;

    // Get only enabled visualizations (respect user choices)
    for (const auto &name : visNames)
    {
        if (visualizationManager_.isVisualizationEnabled(name))
        {
            activeVisualizations.push_back(name);
        }
    }

    int visCount = static_cast<int>(activeVisualizations.size());

    if (visCount == 0)
    {
        return; // No visualizations to render
    }

    // Arrange visualizations in a grid
    int columns = (visCount > 1) ? 2 : 1;
    int rows = (visCount + columns - 1) / columns;

    float visWidth = (availWidth - (MARGIN * (columns - 1))) / columns;
    float visHeight = (availHeight - (MARGIN * (rows - 1))) / rows;

    // Get screen bounds for window clamping
    ImGuiIO &io = ImGui::GetIO();
    const float minWindowVisible = 50.0f; // Minimum window area that must remain visible
    ImRect screenBounds(
        ImVec2(0.0f, 0.0f),
        ImVec2(io.DisplaySize.x, io.DisplaySize.y));

    // Store current window positions for overlap detection
    static std::vector<ImRect> windowRects;
    windowRects.resize(visCount);

    // First pass: Position and size all windows
    for (int i = 0; i < visCount; i++)
    {
        int row = i / columns;
        int col = i % columns;

        float x = startX + col * (visWidth + MARGIN);
        float y = startY + row * (visHeight + MARGIN);

        std::string visName = activeVisualizations[i];

        // Create a unique ID for this window to manage its state
        std::string windowID = visName + "##" + std::to_string(i);

        // Check if window exists already
        bool firstTimePositioning = !ImGui::FindWindowByName(windowID.c_str());

        // Set the initial position and size of the window, but allow resizing
        ImGui::SetNextWindowPos(ImVec2(x, y), firstTimePositioning ? ImGuiCond_Always : ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(visWidth, visHeight), firstTimePositioning ? ImGuiCond_Always : ImGuiCond_FirstUseEver);

        // Set window constraints to maintain reasonable sizes
        ImGui::SetNextWindowSizeConstraints(
            ImVec2(visWidth * 0.5f, visHeight * 0.5f),    // Min size
            ImVec2(availWidth * 0.8f, availHeight * 0.8f) // Max size
        );

        // Add window flags to allow resizing but keep some constraints
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse |
                                 ImGuiWindowFlags_NoBringToFrontOnFocus;

        // Render the window
        bool isEnabled = true; // Local copy to pass to render

        // Render through the visualization manager
        auto visIt = visualizationManager_.getVisualizations().find(visName);
        if (visIt != visualizationManager_.getVisualizations().end())
        {
            ImGui::Begin(windowID.c_str(), nullptr, flags);

            // Store current position and size for overlap detection
            windowRects[i] = ImRect(
                ImGui::GetWindowPos(),
                ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x,
                       ImGui::GetWindowPos().y + ImGui::GetWindowSize().y));

            visIt->second->render(sortManager_, isEnabled);
            ImGui::End();
        }
    }

    // Second pass: Detect and resolve overlaps
    bool overlapsResolved;
    const float separationMargin = 5.0f; // Pixels to separate windows

    // Keep trying to resolve overlaps until there are none or we give up
    int maxIterations = 10;
    int iteration = 0;

    do
    {
        overlapsResolved = true;
        iteration++;

        // Check each window against others
        for (int i = 0; i < visCount; i++)
        {
            for (int j = i + 1; j < visCount; j++)
            {
                // Check if windows overlap
                bool overlaps = windowRects[i].Overlaps(windowRects[j]);

                if (overlaps)
                {
                    overlapsResolved = false;

                    // Get the overlap area
                    ImRect overlap;
                    overlap.Min.x = std::max(windowRects[i].Min.x, windowRects[j].Min.x);
                    overlap.Min.y = std::max(windowRects[i].Min.y, windowRects[j].Min.y);
                    overlap.Max.x = std::min(windowRects[i].Max.x, windowRects[j].Max.x);
                    overlap.Max.y = std::min(windowRects[i].Max.y, windowRects[j].Max.y);

                    // Determine which direction to move (use the dimension with smaller overlap)
                    float overlapWidth = overlap.GetWidth();
                    float overlapHeight = overlap.GetHeight();

                    // Figure out which window is more to the left/top
                    bool iIsLeftOf = windowRects[i].Min.x < windowRects[j].Min.x;
                    bool iIsAbove = windowRects[i].Min.y < windowRects[j].Min.y;

                    if (overlapWidth < overlapHeight)
                    {
                        // Move horizontally
                        float moveAmount = overlapWidth + separationMargin;
                        if (iIsLeftOf)
                        {
                            // Move j right
                            windowRects[j].Min.x += moveAmount;
                            windowRects[j].Max.x += moveAmount;
                        }
                        else
                        {
                            // Move i right
                            windowRects[i].Min.x += moveAmount;
                            windowRects[i].Max.x += moveAmount;
                        }
                    }
                    else
                    {
                        // Move vertically
                        float moveAmount = overlapHeight + separationMargin;
                        if (iIsAbove)
                        {
                            // Move j down
                            windowRects[j].Min.y += moveAmount;
                            windowRects[j].Max.y += moveAmount;
                        }
                        else
                        {
                            // Move i down
                            windowRects[i].Min.y += moveAmount;
                            windowRects[i].Max.y += moveAmount;
                        }
                    }
                }
            }
        }
    } while (!overlapsResolved && iteration < maxIterations);

    // Third pass: Apply the new non-overlapping positions and enforce screen bounds
    for (int i = 0; i < visCount; i++)
    {
        // Clamp window position to screen bounds
        float windowWidth = windowRects[i].GetWidth();
        float windowHeight = windowRects[i].GetHeight();

        // Ensure that at least minWindowVisible pixels of the window are visible on each edge
        float minX = screenBounds.Min.x - (windowWidth - minWindowVisible);
        float maxX = screenBounds.Max.x - minWindowVisible;
        float minY = screenBounds.Min.y; // Allow full height at top for title bar
        float maxY = screenBounds.Max.y - minWindowVisible;

        // Clamp the window position using our custom clamp function
        windowRects[i].Min.x = clamp(windowRects[i].Min.x, minX, maxX);
        windowRects[i].Min.y = clamp(windowRects[i].Min.y, minY, maxY);

        // Recalculate Max after clamping Min
        windowRects[i].Max.x = windowRects[i].Min.x + windowWidth;
        windowRects[i].Max.y = windowRects[i].Min.y + windowHeight;

        // Apply position to window
        std::string visName = activeVisualizations[i];
        std::string windowID = visName + "##" + std::to_string(i);

        // Try to find the window by name
        ImGuiWindow *window = ImGui::FindWindowByName(windowID.c_str());
        if (window)
        {
            // Apply the new position for next frame
            window->Pos = windowRects[i].Min;
        }
    }
}

bool Application::isArrayFullySorted() const
{
    const auto &array = sortManager_.getSortArray().getArray();

    if (array.size() <= 1)
    {
        return true;
    }

    for (size_t i = 1; i < array.size(); i++)
    {
        if (array[i] < array[i - 1])
        {
            return false;
        }
    }

    return true;
}

std::string Application::getAlgorithmComplexity(const std::string &algorithmName) const
{
    if (algorithmName == "Bubble Sort")
    {
        return "Time: O(n²) | Space: O(1)";
    }
    else if (algorithmName == "Quick Sort")
    {
        return "Time: O(n log n) avg, O(n²) worst | Space: O(log n)";
    }
    else if (algorithmName == "Merge Sort")
    {
        return "Time: O(n log n) | Space: O(n)";
    }

    return "Unknown";
}

std::string Application::getAlgorithmDescription(const std::string &algorithmName) const
{
    if (algorithmName == "Bubble Sort")
    {
        return "A simple comparison algorithm that repeatedly steps through the list, compares adjacent elements and swaps them if they are in the wrong order.";
    }
    else if (algorithmName == "Quick Sort")
    {
        return "An efficient divide-and-conquer sorting algorithm that works by selecting a 'pivot' element and partitioning the array around the pivot.";
    }
    else if (algorithmName == "Merge Sort")
    {
        return "A divide-and-conquer algorithm that divides the input array into two halves, recursively sorts them, then merges the sorted halves.";
    }

    return "No description available.";
}

std::string Application::getVisualizationDescription(const std::string &visName) const
{
    if (visName == "Bar Graph")
    {
        return "Represents array elements as vertical bars, with height proportional to the element value.";
    }
    else if (visName == "Heatmap")
    {
        return "Displays array elements as colored cells with a heat gradient based on the element value.";
    }
    else if (visName == "Particle System")
    {
        return "Creates dynamic particles based on array values that flow with physics-based animation.";
    }

    return "No description available.";
}
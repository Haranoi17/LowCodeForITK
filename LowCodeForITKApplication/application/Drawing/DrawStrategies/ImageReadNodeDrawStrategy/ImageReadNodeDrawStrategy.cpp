#include "ImageReadNodeDrawStrategy.hpp"

#include "Application/Drawing/DrawDefaults.hpp"
#include "imgui.h"

#include "imgui_node_editor.h"
#include <format>

namespace ed = ax::NodeEditor;

void ImageReadNodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    if (ImGui::Button("Browse"))
    {
        filebrowser.Open();
    }

    std::wstring selectedPath = filebrowser.GetSelected().native().c_str();
    std::string  pathAsString;

    if (filebrowser.HasSelected())
    {
        for (const auto &wideChar : selectedPath)
        {
            pathAsString.push_back(wideChar);
        }

        if (imageViewNode->imagePath != pathAsString)
        {
            imageViewNode->imagePath = pathAsString;
            imageViewNode->dirty     = true;
        }
    };
}

void ImageReadNodeDrawStrategy::nodeSpecificFunctionalitiesAfterNodeEnd()
{
    if (ImGui::IsItemHovered())
    {
        const auto min = ImGui::GetItemRectMin();
        const auto max = ImGui::GetItemRectMax();

        const auto writeTextPosition = ImVec2{min.x, max.y};
        ImGui::SetCursorPos(writeTextPosition);
        ImGui::TextWrapped(std::format("{}", imageViewNode->imagePath).c_str());
    }

    ed::Suspend();
    filebrowser.Display();
    ed::Resume();
}
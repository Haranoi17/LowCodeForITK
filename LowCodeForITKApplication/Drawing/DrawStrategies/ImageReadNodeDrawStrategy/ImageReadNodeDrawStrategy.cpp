#include "ImageReadNodeDrawStrategy.hpp"

#include "Drawing/DrawDefaults.hpp"
#include "imgui.h"

#include "imgui_node_editor.h"
#include <format>

namespace ed = ax::NodeEditor;

ImageReadNodeDrawStrategy::ImageReadNodeDrawStrategy(ImageReadNode *imageViewNode)
    : BlueprintNodeDrawStrategy{imageViewNode}, m_imageViewNode{imageViewNode}
{
}

void ImageReadNodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    if (ImGui::Button(std::format("Browse {}", m_imageViewNode->id).c_str()))
    {
        m_filebrowser.Open();
    }

    std::wstring selectedPath = m_filebrowser.GetSelected().native().c_str();
    std::string  pathAsString;

    if (m_filebrowser.HasSelected())
    {
        for (const auto &wideChar : selectedPath)
        {
            pathAsString.push_back(wideChar);
        }

        if (m_imageViewNode->imagePath != pathAsString)
        {
            m_imageViewNode->imagePath = pathAsString;
            m_imageViewNode->dirty     = true;
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
        ImGui::TextWrapped(std::format("{}", m_imageViewNode->imagePath).c_str());
    }

    ed::Suspend();
    m_filebrowser.Display();
    ed::Resume();
}
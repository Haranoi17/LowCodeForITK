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

    std::wstring p = m_filebrowser.GetSelected().native().c_str();
    std::string  pp;

    if (m_filebrowser.HasSelected())
    {
        for (const auto &wch : p)
        {
            pp.push_back(wch);
        }

        m_imageViewNode->imagePath = pp;
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
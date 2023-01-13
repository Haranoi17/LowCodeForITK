#include "ImageReadNodeDrawStrategy.hpp"

#include "DrawDefaults.hpp"
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

    if (ImGui::IsItemHovered())
    {
        ImGui::TextWrapped(std::format("{}", m_imageViewNode->imagePath).c_str());
    }
}

void ImageReadNodeDrawStrategy::nodeSpecificFunctionalitiesAfterNodeEnd()
{
    ed::Suspend();
    m_filebrowser.Display();
    ed::Resume();
}
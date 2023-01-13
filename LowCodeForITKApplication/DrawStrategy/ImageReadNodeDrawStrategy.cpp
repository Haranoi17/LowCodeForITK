#include "ImageReadNodeDrawStrategy.hpp"

#include "DrawDefaults.hpp"
#include "imgui.h"

#include "imgui_node_editor.h"
#include <format>

namespace ed = ax::NodeEditor;

ImageReadNodeDrawStrategy::ImageReadNodeDrawStrategy(ImageReadNode *imageViewNode)
    : NodeDrawStrategy{imageViewNode}, m_imageViewNode{imageViewNode}
{
}

void ImageReadNodeDrawStrategy::draw()
{
    static IDType nodeIDForFileBrowsing;

    defaultNodeBegin(m_imageViewNode);

    if (ImGui::Button(std::format("Browse {}", m_imageViewNode->id).c_str()))
    {
        // fileBrowser.Open();
        nodeIDForFileBrowsing = m_imageViewNode->id;
    }

    // std::wstring p = fileBrowser.GetSelected().native().c_str();
    // std::string  pp;

    /* if (fileBrowser.HasSelected())
     {
         for (const auto &wch : p)
         {
             pp.push_back(wch);
         }

         if (node->id == nodeIDForFileBrowsing)
         {
             node->imagePath = pp;
         }
     };*/

    ImGui::TextWrapped(std::format("Path: {}", m_imageViewNode->imagePath).c_str());
    ed::EndNode();
    ed::Suspend();
    // fileBrowser.Display();
    ed::Resume();
}

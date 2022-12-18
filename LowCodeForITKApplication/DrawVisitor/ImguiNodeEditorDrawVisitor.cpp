#include "ImguiNodeEditorDrawVisitor.hpp"
#include "imgui_node_editor.h"
#include <algorithm>
#include <format>

#include "Nodes/ImageViewNode/ImageViewNode.hpp"
#include "Nodes/ReadImageNode/ReadImageNode.hpp"
#include <itkImageFileWriter.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ed = ax::NodeEditor;

void ImguiNodeEditorDrawVisitor::drawNode(const Node *node) const
{
    defaultNodeBegin(node);
    ed::EndNode();
}

void ImguiNodeEditorDrawVisitor::drawReadImageNode(ReadImageNode *node)
{
    static IDType nodeIDForFileBrowsing;

    defaultNodeBegin(node);

    if (ImGui::Button(std::format("Browse {}", node->id).c_str()))
    {
        fileBrowser.Open();
        nodeIDForFileBrowsing = node->id;
    }

    std::wstring p = fileBrowser.GetSelected().native().c_str();
    std::string  pp;

    if (fileBrowser.HasSelected())
    {
        for (const auto &wch : p)
        {
            pp.push_back(wch);
        }

        if (node->id == nodeIDForFileBrowsing)
        {
            node->imagePath = pp;
        }
    };

    ImGui::TextWrapped(std::format("Path: {}", node->imagePath).c_str());
    ed::EndNode();
    ed::Suspend();
    fileBrowser.Display();
    ed::Resume();
}

void ImguiNodeEditorDrawVisitor::drawImageViewNode(ImageViewNode *node)
{
    defaultNodeBegin(node);

    if (node->imagePin->payload.has_value())
    {
        if (!node->texture)
        {
            std::string fileName{std::format("ImageViewNode{}Preview.png", node->id)};
            auto        image  = std::any_cast<itk::SmartPointer<ImageViewNode::ImageType>>(node->imagePin->payload);
            auto        writer = itk::ImageFileWriter<ImageViewNode::ImageType>::New();
            writer->SetFileName(fileName);
            writer->SetInput(image);
            writer->Update();

            int component = 0;
            if (auto data = stbi_load(fileName.c_str(), &node->width, &node->height, &component, 4))
            {
                node->texture = imguiApplication->CreateTexture(data, node->width, node->height);
            }
        }
        else
        {
            ImGui::Image((void *)node->texture, ImVec2(node->width, node->height));
        }
    }
    else
    {
        if (node->texture)
        {
            imguiApplication->DestroyTexture(node->texture);
            node->texture = nullptr;
        }
    }
    ed::EndNode();
}

void ImguiNodeEditorDrawVisitor::drawInputPin(const Pin *pin) const
{
    ed::BeginPin(pin->id, ed::PinKind::Input);
    ImGui::Text(std::format("->{}", pin->name).c_str());
    ed::EndPin();
}

void ImguiNodeEditorDrawVisitor::drawOutputPin(const Pin *pin) const
{
    ed::BeginPin(pin->id, ed::PinKind::Output);
    ImGui::Text(std::format("{}->", pin->name).c_str());
    ed::EndPin();
}

void ImguiNodeEditorDrawVisitor::defaultNodeBegin(const Node *node) const
{
    ed::BeginNode(node->id);
    ImGui::Text(std::format("{}", node->name).c_str());

    ImGui::BeginGroup();
    std::ranges::for_each(node->m_inputPins, [&](const std::unique_ptr<Pin> &pin) { drawInputPin(pin.get()); });
    ImGui::EndGroup();
    node->m_inputPins.empty() ? ImGui::SameLine(200) : ImGui::SameLine();
    ImGui::BeginGroup();
    std::ranges::for_each(node->m_outputPins, [&](const std::unique_ptr<Pin> &pin) { drawOutputPin(pin.get()); });
    ImGui::EndGroup();
}

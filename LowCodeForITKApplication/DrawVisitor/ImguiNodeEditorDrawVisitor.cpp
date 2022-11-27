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

void ImguiNodeEditorDrawVisitor::drawReadImageNode(ReadImageNode *node) const
{
    defaultNodeBegin(node);

    char buf[128];
    strcpy(buf, node->imagePath.c_str());
    ImGui::PushItemWidth(300);
    if (ImGui::InputText("imagePath", buf, 128))
    {
        node->imagePath = buf;
    }

    ed::EndNode();
}

void ImguiNodeEditorDrawVisitor::drawImageViewNode(const ImageViewNode *node) const
{
    static ImTextureID texture;
    static int         width, height;

    defaultNodeBegin(node);

    if (node->imagePin->payload.has_value())
    {
        if (!texture)
        {
            std::string fileName{std::format("ImageViewNode{}Preview.png", node->id)};
            auto        image  = std::any_cast<itk::SmartPointer<ImageViewNode::ImageType>>(node->imagePin->payload);
            auto        writer = itk::ImageFileWriter<ImageViewNode::ImageType>::New();
            writer->SetFileName(fileName);
            writer->SetInput(image);
            writer->Update();

            int component = 0;
            if (auto data = stbi_load(fileName.c_str(), &width, &height, &component, 4))
            {
                texture = imguiApplication->CreateTexture(data, width, height);
            }
        }
        else
        {
            ImGui::Image((void *)texture, ImVec2(width, height));
        }
    }
    else
    {
        if (texture)
        {
            imguiApplication->DestroyTexture(texture);
            texture = nullptr;
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
    ImGui::SameLine();
    ImGui::BeginGroup();
    std::ranges::for_each(node->m_outputPins, [&](const std::unique_ptr<Pin> &pin) { drawOutputPin(pin.get()); });
    ImGui::EndGroup();
}

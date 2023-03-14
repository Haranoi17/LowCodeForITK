#pragma once
#include "Nodes/ImageReadNode/ImageReadNode.hpp"
#include "Nodes/Interface/Node.hpp"
#include <imgui.h>
#include <itkImage.h>

class RGBANode : public Node
{
  public:
    using PixelType = ImageReadNode::PixelType;
    using ImageType = ImageReadNode::ImageType;

    inline static std::string typeName = "RGBANode";
    RGBANode()                         = default;
    RGBANode(UniqueIDProvider *idProvider, std::string_view name = typeName);

    Pin *rgbaPin;

    PixelType rgbaValues;
};

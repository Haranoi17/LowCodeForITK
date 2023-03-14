#pragma once
#include "Nodes/ImageReadNode/ImageReadNode.hpp"
#include "Nodes/Interface/Node.hpp"
#include <imgui.h>
#include <itkImage.h>

class ImageViewNode : public Node
{
  public:
    using PixelType = ImageReadNode::PixelType;
    using ImageType = ImageReadNode::ImageType;

    inline static std::string typeName = "ImageViewNode";
    ImageViewNode(UniqueIDProvider *idProvider, std::string_view name = typeName);
    ImageViewNode() = default;

    void deserialize(json data);

    Pin *imagePin;

    ImTextureID texture{};
    int         width{};
    int         height{};
};

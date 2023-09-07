#pragma once
#include "Logic/Nodes/ImageReadNode/ImageReadNode.hpp"
#include "Logic/Nodes/Node.hpp"
#include <imgui.h>
#include <itkImage.h>

class RGBANode : public Node
{
  public:
    using PixelType = ImageReadNode::PixelType;
    using ImageType = ImageReadNode::ImageType;

    inline static std::string typeName = "RGBANode";

    RGBANode(UniqueIDProvider *idProvider);

    void populateOutputPins() override;

    json serialize() override;
    void deserialize(json data) override;

    Pin *rgbaPin{};

    PixelType rgbaValues{};
};
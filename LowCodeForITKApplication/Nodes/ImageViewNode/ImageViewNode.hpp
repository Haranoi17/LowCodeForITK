#pragma once
#include "Nodes/Interface/Node.hpp"
#include "Nodes\ReadImageNode\ReadImageNode.hpp"
#include <itkImage.h>

class ImageViewNode : public Node
{
  public:
    using PixelType = ReadImageNode::PixelType;
    using ImageType = ReadImageNode::ImageType;

    ImageViewNode(UniqueIDProvider *idProvider, std::string_view name = "ImageView");

    void acceptDrawVisitor(DrawVisitor *drawVisitor) override;

    Pin *imagePin;
};

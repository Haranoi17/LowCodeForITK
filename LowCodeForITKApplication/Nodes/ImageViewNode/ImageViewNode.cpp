#include "ImageViewNode.hpp"
#include "Pins/ImagePin/ImagePin.hpp"

ImageViewNode::ImageViewNode(UniqueIDProvider *idProvider, std::string_view name) : Node{idProvider, name}
{
    m_inputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    imagePin = m_inputPins.back().get();
}

void ImageViewNode::acceptDrawVisitor(DrawVisitor *drawVisitor)
{
    drawVisitor->drawImageViewNode(this);
}

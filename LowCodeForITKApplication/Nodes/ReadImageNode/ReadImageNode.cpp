#include "ReadImageNode.hpp"
#include "Pins/ImagePin/ImagePin.hpp"

#include <itkImageFileReader.h>

ReadImageNode::ReadImageNode(UniqueIDProvider *idProvider, std::string_view name) : Node{idProvider, name}
{

    m_outputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    imagePin = m_outputPins.back().get();
}

void ReadImageNode::acceptDrawVisitor(DrawVisitor *drawVisitor)
{
    drawVisitor->drawReadImageNode(this);
}

void ReadImageNode::populateOutputPins()
{
    // imagePin->payload = itk::ReadImage<ImageType>(imagePath);
    imagePin->payload = itk::ReadImage<ImageType>("data/testImage.png");
}

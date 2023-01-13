#include "ImageReadNode.hpp"
#include "Pins/ImagePin/ImagePin.hpp"

#include <itkImageFileReader.h>

ImageReadNode::ImageReadNode(UniqueIDProvider *idProvider, std::string_view name) : Node{idProvider, name}
{

    m_outputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    imagePin = m_outputPins.back().get();
}

void ImageReadNode::populateOutputPins()
{
    if (imagePath.empty())
    {
        return;
    }

    imagePin->payload = itk::ReadImage<ImageType>(imagePath);
}

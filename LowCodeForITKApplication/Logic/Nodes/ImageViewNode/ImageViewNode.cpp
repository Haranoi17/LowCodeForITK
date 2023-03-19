#include "ImageViewNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

#include <itkAddImageFilter.h>
#include <itkImageDuplicator.h>
#include <itkImageIterator.h>
#include <itkMultiplyImageFilter.h>

ImageViewNode::ImageViewNode(UniqueIDProvider *idProvider, std::string_view name) : Node{idProvider, name}
{
    inputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    imagePin = inputPins.back().get();
}

void ImageViewNode::calculate()
{
    auto image = std::any_cast<itk::SmartPointer<ImageType>>(imagePin->payload);

    auto size = image->GetLargestPossibleRegion().GetSize();
    width     = size[0];
    height    = size[1];

    flatImageArray = std::make_unique<PixelType::ComponentType[]>(width * height * PixelType::Dimension);

    auto it          = itk::ImageRegionIterator<ImageType>(image, image->GetLargestPossibleRegion());
    auto flatArrayIt = 0;
    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
        flatImageArray[flatArrayIt]     = it.Value().GetRed();
        flatImageArray[flatArrayIt + 1] = it.Value().GetGreen();
        flatImageArray[flatArrayIt + 2] = it.Value().GetBlue();
        flatImageArray[flatArrayIt + 3] = it.Value().GetAlpha();

        flatArrayIt += PixelType::Dimension;
    }

    imageChanged = true;
}

void ImageViewNode::deserialize(json data)
{
    Node::deserialize(data);
    imagePin = inputPins.back().get();
}
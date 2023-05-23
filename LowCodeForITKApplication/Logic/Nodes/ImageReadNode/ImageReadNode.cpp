#include "ImageReadNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

#include <itkImageFileReader.h>
#include <itkImageRegion.h>

ImageReadNode::ImageReadNode(UniqueIDProvider *idProvider) : Node{idProvider, typeName}
{
    outputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    imagePin = outputPins.back().get();
}

void ImageReadNode::populateOutputPins()
{
    if (imagePath.empty())
    {
        return;
    }

    try
    {
        imagePin->payload = itk::ReadImage<ImageType>(imagePath);
    }
    catch (std::exception e)
    {
        auto          mockImage = ImageType::New();
        itk::Size<2U> size{};
        size[0] = 256;
        size[1] = 256;

        mockImage->SetRegions(ImageType::SizeType(size));
        mockImage->Allocate();
        imagePin->payload = mockImage;
    }
}

json ImageReadNode::serialize()
{
    json serializedImageReadNode         = Node::serialize();
    serializedImageReadNode["imagePath"] = imagePath;
    return serializedImageReadNode;
}

void ImageReadNode::deserialize(json data)
{
    Node::deserialize(data);
    imagePath = data["imagePath"];

    imagePin = outputPins.back().get();
}

#include "ImageReadNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

#include <itkImageFileReader.h>

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

    imagePin->payload = itk::ReadImage<ImageType>(imagePath);
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

    imagePin = outputPins.back().get(); // this is baaaaad...
}

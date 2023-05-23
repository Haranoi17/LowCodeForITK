#include "ImageSaveNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

#include "itkImageFileWriter.h"

ImageSaveNode::ImageSaveNode(UniqueIDProvider *idProvider) : Node{idProvider, typeName}
{
    inputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    imagePin = inputPins.back().get();
}

void ImageSaveNode::calculate()
{
    imageToSave = std::any_cast<ImageType::Pointer>(imagePin->payload);
}

void ImageSaveNode::save()
{
    auto writer = itk::ImageFileWriter<ImageType>::New();

    writer->SetInput(imageToSave);
    writer->SetFileName(fileName);

    try
    {
        writer->Update();
    }
    catch (std::exception e)
    {
        std::cout << "Check extention you are saving with eg: .png, .jpg" << '\n';
    }
}

json ImageSaveNode::serialize()
{
    json serializedNode        = Node::serialize();
    serializedNode["fileName"] = fileName;
    return serializedNode;
}

void ImageSaveNode::deserialize(json data)
{
    Node::deserialize(data);
    fileName = data["fileName"];

    imagePin = inputPins.front().get();
}

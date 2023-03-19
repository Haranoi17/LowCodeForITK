#include "RGBANode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

#include <itkAddImageFilter.h>
#include <itkImageDuplicator.h>
#include <itkImageIterator.h>
#include <itkMultiplyImageFilter.h>

RGBANode::RGBANode(UniqueIDProvider *idProvider, std::string_view name) : Node{idProvider, name}
{
    outputPins.emplace_back(std::make_unique<RGBAPin>(idProvider, this));
    rgbaPin = outputPins.back().get();
}

void RGBANode::populateOutputPins()
{
    rgbaPin->payload = rgbaValues;
}

json RGBANode::serialize()
{
    json serializedRGBANode     = Node::serialize();
    serializedRGBANode["red"]   = rgbaValues.GetRed();
    serializedRGBANode["green"] = rgbaValues.GetGreen();
    serializedRGBANode["blue"]  = rgbaValues.GetBlue();
    serializedRGBANode["alpha"] = rgbaValues.GetAlpha();

    return serializedRGBANode;
}

void RGBANode::deserialize(json data)
{
    Node::deserialize(data);
    rgbaValues.SetRed(data["red"]);
    rgbaValues.SetGreen(data["green"]);
    rgbaValues.SetBlue(data["blue"]);
    rgbaValues.SetAlpha(data["alpha"]);

    rgbaPin = outputPins.back().get();
}

TintNode::TintNode(UniqueIDProvider *idProvider, std::string_view name) : Node{idProvider, name}
{
    inputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    imagePin = inputPins.back().get();

    inputPins.emplace_back(std::make_unique<RGBAPin>(idProvider, this));
    colorPin = inputPins.back().get();

    inputPins.emplace_back(std::make_unique<PercentagePin>(idProvider, this));
    percentagePin = inputPins.back().get();

    outputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    processedImagePin = outputPins.back().get();
}

void TintNode::calculate()
{
    auto percentage = std::any_cast<float>(percentagePin->payload);
    auto color      = std::any_cast<PixelType>(colorPin->payload);
    auto image      = std::any_cast<itk::SmartPointer<ImageType>>(imagePin->payload);

    auto imageDuplicator = itk::ImageDuplicator<ImageType>::New();
    imageDuplicator->SetInputImage(image);
    imageDuplicator->Update();

    auto imageCopy = imageDuplicator->GetOutput();

    itk::ImageRegionIterator<ImageType> it{imageCopy, imageCopy->GetRequestedRegion()};

    PixelType modifiedColor;
    modifiedColor.SetAlpha(color.GetAlpha());
    modifiedColor.SetRed(percentage * color.GetRed());
    modifiedColor.SetGreen(percentage * color.GetGreen());
    modifiedColor.SetBlue(percentage * color.GetBlue());

    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
        PixelType originalImagePixel = it.Value();
        PixelType modifiedImagePixel;
        modifiedImagePixel.SetAlpha(originalImagePixel.GetAlpha());
        modifiedImagePixel.SetRed((1.0f - percentage) * originalImagePixel.GetRed());
        modifiedImagePixel.SetGreen((1.0f - percentage) * originalImagePixel.GetGreen());
        modifiedImagePixel.SetBlue((1.0f - percentage) * originalImagePixel.GetBlue());
        it.Set(modifiedImagePixel + modifiedColor);
    }
    processedImage = imageCopy;
}

void TintNode::populateOutputPins()
{
    processedImagePin->payload = processedImage;
}

PercentageNode::PercentageNode(UniqueIDProvider *idProvider, std::string_view name) : Node{idProvider, name}
{
    outputPins.emplace_back(std::make_unique<PercentagePin>(idProvider, this));
    percentagePin = outputPins.back().get();
}

json PercentageNode::serialize()
{
    json serializedPercentageNode          = Node::serialize();
    serializedPercentageNode["percentage"] = percentage;

    return serializedPercentageNode;
}

void PercentageNode::deserialize(json data)
{
    Node::deserialize(data);
    percentage = data["percentage"];

    percentagePin = outputPins.back().get();
}

#include "BinaryThresholdNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

#include "itkBinaryThresholdImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkRGBToLuminanceImageFilter.h"

BinaryThresholdNode::BinaryThresholdNode(UniqueIDProvider *idProvider) : Node{idProvider, typeName}
{
    inputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    inputImagePin = inputPins.back().get();

    inputPins.emplace_back(std::make_unique<FloatingPointValuePin>(idProvider, this, "lower"));
    minPin = inputPins.back().get();

    inputPins.emplace_back(std::make_unique<FloatingPointValuePin>(idProvider, this, "upper"));
    maxPin = inputPins.back().get();

    outputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    outputImagePin = outputPins.back().get();
}

void BinaryThresholdNode::calculate()
{
    auto inputImage = std::any_cast<ImageType::Pointer>(inputImagePin->payload);
    auto lower      = std::any_cast<float>(minPin->payload);
    auto upper      = std::any_cast<float>(maxPin->payload);

    if (upper < lower)
    {
        lower = upper;
    }

    using GrayScaleImageType = itk::Image<float>;

    using ToGrayFilter = itk::RGBToLuminanceImageFilter<ImageType, GrayScaleImageType>;
    auto toGray        = ToGrayFilter::New();
    toGray->SetInput(inputImage);
    toGray->Update();

    auto image = toGray->GetOutput();

    using FilterType = itk::BinaryThresholdImageFilter<GrayScaleImageType, GrayScaleImageType>;
    auto filter      = FilterType::New();
    filter->SetInput(image);
    filter->SetLowerThreshold(lower);
    filter->SetUpperThreshold(upper);
    filter->SetOutsideValue(itk::NumericTraits<ImageType::PixelType::ComponentType>::min());
    filter->SetInsideValue(itk::NumericTraits<ImageType::PixelType::ComponentType>::max());

    filter->Update();

    using CastType = itk::CastImageFilter<GrayScaleImageType, ImageType>;
    auto caster    = CastType::New();
    caster->SetInput(filter->GetOutput());

    caster->Update();

    outputImage = caster->GetOutput();
}

void BinaryThresholdNode::populateOutputPins()
{
    outputImagePin->payload = outputImage;
}

json BinaryThresholdNode::serialize()
{
    json serializedNode = Node::serialize();
    return serializedNode;
}

void BinaryThresholdNode::deserialize(json data)
{
    Node::deserialize(data);

    inputImagePin = inputPins.at(0).get();
    minPin        = inputPins.at(1).get();
    maxPin        = inputPins.at(2).get();

    outputImagePin = outputPins.back().get();
}

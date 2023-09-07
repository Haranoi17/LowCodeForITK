#include "GaussianBlurNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"
#include "itkRescaleIntensityImageFilter.h"
#include <itkCastImageFilter.h>
#include <itkImageRegionIterator.h>
#include <itkSmoothingRecursiveGaussianImageFilter.h>

#include "Logic/utilities.hpp"

GaussianBlurNode::GaussianBlurNode(UniqueIDProvider *idProvider) : Node{idProvider, typeName}
{
    inputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    inputImagePin = inputPins.back().get();

    inputPins.emplace_back(std::make_unique<FloatingPointValuePin>(idProvider, this));
    sigmaPin = inputPins.back().get();

    outputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    outputImagePin = outputPins.back().get();
}

void GaussianBlurNode::calculate()
{
    auto image = std::any_cast<itk::SmartPointer<ImageType>>(inputImagePin->payload);
    auto sigma = std::any_cast<float>(sigmaPin->payload);
    if (sigma <= 0)
    {
        outputImage = image;
        return;
    }

    using GaussianFunctionType = itk::SmoothingRecursiveGaussianImageFilter<ImageType, ImageType>;
    GaussianFunctionType::Pointer gaussianFunction = GaussianFunctionType::New();

    gaussianFunction->SetInput(image);
    gaussianFunction->SetSigma(sigma);

    gaussianFunction->Update();

    outputImage = gaussianFunction->GetOutput();
}

void GaussianBlurNode::populateOutputPins()
{
    outputImagePin->payload = outputImage;
}

void GaussianBlurNode::deserialize(json data)
{
    Node::deserialize(data);

    inputImagePin = inputPins.at(to_underlying(InputPinIndex::imagePin)).get();
    sigmaPin      = inputPins.at(to_underlying(InputPinIndex::sigmaPin)).get();

    outputImagePin = outputPins.at(to_underlying(OutputPinIndex::imagePin)).get();
}

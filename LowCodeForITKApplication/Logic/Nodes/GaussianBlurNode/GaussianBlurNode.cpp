#include "GaussianBlurNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"
#include "itkRescaleIntensityImageFilter.h"
#include <itkCastImageFilter.h>
#include <itkImageRegionIterator.h>
#include <itkSmoothingRecursiveGaussianImageFilter.h>

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

    ImageType::RegionType region = image->GetBufferedRegion();

    ImageType::Pointer output = ImageType::New();

    output->SetRegions(region);
    output->SetOrigin(image->GetOrigin());
    output->SetSpacing(image->GetSpacing());
    output->Allocate();

    using GFunctionType                     = itk::SmoothingRecursiveGaussianImageFilter<ImageType, ImageType>;
    GFunctionType::Pointer gaussianFunction = GFunctionType::New();
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

    inputImagePin  = inputPins.at(0).get();
    sigmaPin       = inputPins.at(1).get();
    outputImagePin = outputPins.at(0).get();
}

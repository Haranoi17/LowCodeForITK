#include "EdgeDetectionNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

#include <itkCannyEdgeDetectionImageFilter.h>
#include <itkRGBToLuminanceImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkScalarToRGBColormapImageFilter.h>

#include "itkImage.h"
#include "itkRGBAPixel.h"
#include "itkRGBToLuminanceImageAdaptor.h"
#include "itkRGBToLuminanceImageFilter.h"
#include <itkImageAdaptor.h>

#include <itkComposeImageFilter.h>
#include <itkImageFileWriter.h>

#include <itkCastImageFilter.h>
#include <itkImageDuplicator.h>
#include <itkRGBToLuminanceImageAdaptor.h>
#include <itkScalarToRGBPixelFunctor.h>
#include <limits>

EdgeDetectionNode::EdgeDetectionNode(UniqueIDProvider *idProvider) : Node{idProvider, typeName}
{
    inputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    inputImagePin = inputPins.back().get();

    inputPins.emplace_back(std::make_unique<PercentagePin>(idProvider, this, "variance"));
    varianceFloatPin = inputPins.back().get();

    inputPins.emplace_back(std::make_unique<PercentagePin>(idProvider, this, "lowThreshold"));
    lowThresholdPin = inputPins.back().get();

    inputPins.emplace_back(std::make_unique<PercentagePin>(idProvider, this, "highThreshold"));
    highThresholdPin = inputPins.back().get();

    outputPins.emplace_back(std::make_unique<ImagePin>(idProvider, this));
    outputImagePin = outputPins.back().get();
}

void EdgeDetectionNode::calculate()
{
    auto image         = std::any_cast<itk::SmartPointer<ImageType>>(inputImagePin->payload);
    auto variance      = std::any_cast<float>(varianceFloatPin->payload);
    auto lowThreshold  = std::any_cast<float>(lowThresholdPin->payload);
    auto highThreshold = std::any_cast<float>(highThresholdPin->payload);

    using GrayScalePixelType = unsigned char;
    using GrayScaleImageType = itk::Image<GrayScalePixelType, ImageType::ImageDimension>;

    using GrayScaleFilter = itk::RGBToLuminanceImageFilter<ImageType, GrayScaleImageType>;
    auto grayscaleFilter  = GrayScaleFilter::New();
    grayscaleFilter->SetInput(image);
    grayscaleFilter->Update();

    auto grayscaleImage = grayscaleFilter->GetOutput();

    using FloatGrayScalePixelType = float;
    using FloatGrayScaleImageType = itk::Image<FloatGrayScalePixelType, ImageType::ImageDimension>;
    using RescaleToFloatType      = itk::RescaleIntensityImageFilter<GrayScaleImageType, FloatGrayScaleImageType>;
    auto rescalerToFloat          = RescaleToFloatType::New();
    rescalerToFloat->SetInput(grayscaleImage);
    rescalerToFloat->SetOutputMinimum(0.0f);
    rescalerToFloat->SetOutputMaximum(1.0f);
    rescalerToFloat->Update();

    auto floatImage = rescalerToFloat->GetOutput();

    using EdgeDetectionFilter = itk::CannyEdgeDetectionImageFilter<FloatGrayScaleImageType, FloatGrayScaleImageType>;
    auto edgeDetectionFilter  = EdgeDetectionFilter::New();
    edgeDetectionFilter->SetInput(floatImage);
    edgeDetectionFilter->SetVariance(variance);
    edgeDetectionFilter->SetLowerThreshold(lowThreshold);
    edgeDetectionFilter->SetUpperThreshold(highThreshold);
    edgeDetectionFilter->Update();

    auto edgeDetectedImage = edgeDetectionFilter->GetOutput();

    using RescaleToImageType = itk::RescaleIntensityImageFilter<FloatGrayScaleImageType, GrayScaleImageType>;
    auto rescalerToImage     = RescaleToImageType::New();
    rescalerToImage->SetInput(edgeDetectedImage);
    rescalerToImage->SetOutputMinimum(std::numeric_limits<GrayScalePixelType>::min());
    rescalerToImage->SetOutputMaximum(std::numeric_limits<GrayScalePixelType>::max());
    rescalerToImage->Update();

    auto result = rescalerToImage->GetOutput();

    using FinalFilter = itk::ComposeImageFilter<GrayScaleImageType, ImageType>;
    auto fil          = FinalFilter::New();
    fil->SetInput(0, result);

    auto rgba_channel = GrayScaleImageType::New();
    rgba_channel->SetRegions(result->GetLargestPossibleRegion());
    rgba_channel->Allocate();
    rgba_channel->FillBuffer(0);
    fil->SetInput(1, rgba_channel);
    auto rgba_channel2 = GrayScaleImageType::New();
    rgba_channel2->SetRegions(result->GetLargestPossibleRegion());
    rgba_channel2->Allocate();
    rgba_channel2->FillBuffer(0);
    fil->SetInput(2, rgba_channel2);

    auto a_channel = GrayScaleImageType::New();
    a_channel->SetRegions(result->GetLargestPossibleRegion());
    a_channel->Allocate();
    a_channel->FillBuffer(255);
    fil->SetInput(3, a_channel);
    fil->Update();

    processedImage = fil->GetOutput();
}

void EdgeDetectionNode::populateOutputPins()
{
    outputImagePin->payload = processedImage;
}

void EdgeDetectionNode::deserialize(json data)
{
    Node::deserialize(data);
    inputImagePin    = inputPins.at(0).get();
    varianceFloatPin = inputPins.at(1).get();
    lowThresholdPin  = inputPins.at(2).get();
    highThresholdPin = inputPins.at(3).get();
    outputImagePin   = outputPins.at(0).get();
}

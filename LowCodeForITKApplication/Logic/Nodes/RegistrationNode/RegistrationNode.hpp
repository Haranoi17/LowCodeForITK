#pragma once
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"

#include <itkImage.h>
#include <itkRGBAPixel.h>

// using PixelType = itk::RGBAPixel<unsigned char>;
// using ImageType = itk::Image<PixelType, 2>;
//
// template <typename FromImageType>
// ImageType::Pointer castToProgramImageType(typename FromImageType::Pointer image)
//{
//     using CastFilter = itk::CastImageFilter<FromImageType, ImageType>;
//     auto castFilter  = CastFilter::New();
//     castFilter->SetInput(toGrayFilter->GetOutput());
//     castFilter->Update();
//
//     return castFilter->GetOutput();
// }
//
// template <typename ToImageType>
// ToImageType castProgramImageTypeToGrayScale(ImageType::Pointer image)
//{
//     constexpr unsigned int Dimension = ImageType::ImageDimension;
//     using PixelType                  = float;
//
//     using GrayScaleType = itk::Image<PixelType, Dimension>;
//     using ToOtherFilter = itk::RGBToLuminanceImageFilter<ImageType, GrayScaleType>;
//     auto toOtherFilter  = ToGrayFilter::New();
//
//     toOtherFilter->SetInput(referenceImage);
//     toOtherFilter->Update();
//
//     return toOtherFilter->GetOutput();
// }

class RegistrationNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    inline static std::string typeName = "RegistrationNode";
    RegistrationNode(UniqueIDProvider *idProvider);
    RegistrationNode() = default;

    void calculate() override;
    void populateOutputPins() override;

    json serialize() override;
    void deserialize(json data) override;

    ImageType::Pointer registeredImage{};

    Pin *referenceImagePin{};
    Pin *registeringImagePin{};

    Pin *learningRatePin{};
    Pin *relaxationFactorPin{};
    Pin *minStepLengthPin{};
    Pin *iterationCountPin{};
    Pin *scaleDividorPin{};
    Pin *anglePin{};

    Pin *outputImagePin{};
};

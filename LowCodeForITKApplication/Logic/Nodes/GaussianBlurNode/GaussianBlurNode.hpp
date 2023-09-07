#pragma once

#include "Logic/Nodes/Node.hpp"
#include <itkImage.h>
#include <itkRGBAPixel.h>

class GaussianBlurNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    inline static std::string typeName = "GaussianBlurNode";
    GaussianBlurNode(UniqueIDProvider *idProvider);

    void calculate() override;
    void populateOutputPins() override;

    void deserialize(json data) override;

  private:
    enum class InputPinIndex : short
    {
        imagePin,
        sigmaPin,
    };

    enum class OutputPinIndex : short
    {
        imagePin
    };

    Pin *inputImagePin;
    Pin *sigmaPin;
    Pin *outputImagePin;

    itk::SmartPointer<ImageType> outputImage;
};


#pragma once
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"

#include <itkImage.h>
#include <itkRGBAPixel.h>

class EdgeDetectionNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    inline static std::string typeName = "EdgeDetectionNode";
    EdgeDetectionNode(UniqueIDProvider *idProvider, std::string_view name = typeName);
    EdgeDetectionNode() = default;

    void calculate() override;
    void populateOutputPins() override;

    void deserialize(json data) override;

  private:
    Pin *inputImagePin;
    Pin *outputImagePin;

    Pin                         *varianceFloatPin;
    Pin                         *lowThresholdPin;
    Pin                         *highThresholdPin;
    itk::SmartPointer<ImageType> processedImage{};
};

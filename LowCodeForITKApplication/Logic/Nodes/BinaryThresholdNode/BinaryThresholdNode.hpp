#pragma once
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"

#include <itkImage.h>
#include <itkRGBAPixel.h>

class BinaryThresholdNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    inline static std::string typeName = "BinaryThresholdNode";
    BinaryThresholdNode(UniqueIDProvider *idProvider);

    void calculate() override;
    void populateOutputPins() override;

    json serialize() override;
    void deserialize(json data) override;

    Pin *inputImagePin{};
    Pin *minPin{};
    Pin *maxPin{};
    Pin *outputImagePin{};

    ImageType::Pointer outputImage{};
};

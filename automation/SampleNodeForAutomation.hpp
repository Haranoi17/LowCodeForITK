#pragma once
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"

#include <itkImage.h>
#include <itkRGBAPixel.h>

class SampleNodeForAutomation : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    inline static std::string typeName = "SampleNodeForAutomation";
    SampleNodeForAutomation(UniqueIDProvider *idProvider);
    SampleNodeForAutomation() = default;

    void calculate() override;
    void populateOutputPins() override;

    json serialize() override;
    void deserialize(json data) override;
};

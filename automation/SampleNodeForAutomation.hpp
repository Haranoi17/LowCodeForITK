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
    SampleNodeForAutomation(UniqueIDProvider *idProvider, std::string_view name = typeName);
    SampleNodeForAutomation() = default;

    void calculate() override;
    void populateOutputPins() override;

    void deserialize(json data) override;
};

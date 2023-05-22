#pragma once
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"

#include <itkImage.h>
#include <itkRGBAPixel.h>

class FloatingPoinValueNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    inline static std::string typeName = "FloatingPoinValueNode";
    FloatingPoinValueNode(UniqueIDProvider *idProvider);
    FloatingPoinValueNode() = default;

    void populateOutputPins() override;

    void deserialize(json data) override;

    float value{};
    Pin  *valuePin;
};

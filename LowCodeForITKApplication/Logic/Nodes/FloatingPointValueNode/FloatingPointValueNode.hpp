#pragma once
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"

#include <itkImage.h>
#include <itkRGBAPixel.h>

class FloatingPointValueNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    inline static std::string typeName = "FloatingPoinValueNode";
    FloatingPointValueNode(UniqueIDProvider *idProvider);
    FloatingPointValueNode() = default;

    void populateOutputPins() override;

    json serialize() override;
    void deserialize(json data) override;

    float value{};
    Pin  *valuePin;
};

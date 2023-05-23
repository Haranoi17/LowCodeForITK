#pragma once
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"

#include <itkImage.h>
#include <itkRGBAPixel.h>

class ImageSaveNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    inline static std::string typeName = "ImageSaveNode";
    ImageSaveNode(UniqueIDProvider *idProvider);
    ImageSaveNode() = default;

    void calculate() override;

    json serialize() override;
    void deserialize(json data) override;

    void save();

    std::string        fileName{};
    ImageType::Pointer imageToSave;
    Pin               *imagePin{};
};

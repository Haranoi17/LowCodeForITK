#pragma once
#include "Nodes/Interface/Node.hpp"
#include <itkImage.h>
#include <itkRGBAPixel.h>

class ImageReadNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    inline static std::string typeName = "ImageReadNode";
    ImageReadNode(UniqueIDProvider *idProvider, std::string_view name = typeName);
    ImageReadNode() = default;

    void populateOutputPins() override;

    void deserialize(json data) override
    {
        Node::deserialize(data);
        imagePin = m_outputPins.back().get();
    }

    std::string imagePath;

  private:
    Pin *imagePin;
};

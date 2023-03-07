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

    json serialize() override
    {
        json serializedImageReadNode         = Node::serialize();
        serializedImageReadNode["imagePath"] = imagePath;
        return serializedImageReadNode;
    }

    void deserialize(json data) override
    {
        Node::deserialize(data);
        imagePath = data["imagePath"];

        imagePin = m_outputPins.back().get(); // this is baaaaad...
    }

    std::string imagePath;

  private:
    Pin *imagePin;
};

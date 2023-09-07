#pragma once
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"

#include <itkImage.h>
#include <itkRGBAPixel.h>
#include <Logic/Nodes/ImageReadNode/ImageReadNode.hpp>

class TintNode : public Node
{
  public:
    using PixelType = ImageReadNode::PixelType;
    using ImageType = ImageReadNode::ImageType;

    inline static std::string typeName = "TintNode";

    TintNode(UniqueIDProvider *idProvider, std::string_view name = typeName);

    void calculate() override;
    void populateOutputPins() override;

    void deserialize(json data)
    {
        Node::deserialize(data);
        imagePin          = inputPins.at(0).get();
        colorPin          = inputPins.at(1).get();
        percentagePin     = inputPins.at(2).get();
        processedImagePin = outputPins.back().get();
    }

    Pin *percentagePin{};
    Pin *colorPin{};
    Pin *imagePin{};
    Pin *processedImagePin{};

  private:
    itk::SmartPointer<ImageType> processedImage{};
};
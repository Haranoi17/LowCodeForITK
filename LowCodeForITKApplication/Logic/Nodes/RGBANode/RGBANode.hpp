#pragma once
#include "Logic/Nodes/ImageReadNode/ImageReadNode.hpp"
#include "Logic/Nodes/Node.hpp"
#include <imgui.h>
#include <itkImage.h>

class RGBANode : public Node
{
  public:
    using PixelType = ImageReadNode::PixelType;
    using ImageType = ImageReadNode::ImageType;

    inline static std::string typeName = "RGBANode";

    RGBANode() = default;
    RGBANode(UniqueIDProvider *idProvider);

    void populateOutputPins() override;

    json serialize() override;
    void deserialize(json data) override;

    Pin *rgbaPin{};

    PixelType rgbaValues{};
};

class PercentageNode : public Node
{
  public:
    inline static std::string typeName = "PercentageNode";

    PercentageNode() = default;
    PercentageNode(UniqueIDProvider *idProvider, std::string_view name = typeName);

    void populateOutputPins() override
    {
        percentagePin->payload = percentage;
    }

    json serialize() override;
    void deserialize(json data);

    Pin  *percentagePin{};
    float percentage{};
};

class TintNode : public Node
{
  public:
    using PixelType = ImageReadNode::PixelType;
    using ImageType = ImageReadNode::ImageType;

    inline static std::string typeName = "TintNode";

    TintNode() = default;
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
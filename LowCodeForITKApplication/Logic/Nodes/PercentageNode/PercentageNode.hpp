#pragma once
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"

#include <itkImage.h>
#include <itkRGBAPixel.h>

class PercentageNode : public Node
{
  public:
    inline static std::string typeName = "PercentageNode";

    PercentageNode() = default;
    PercentageNode(UniqueIDProvider *idProvider, std::string_view name = typeName);

    void populateOutputPins() override;

    json serialize() override;
    void deserialize(json data);

    Pin  *percentagePin{};
    float percentage{};
};
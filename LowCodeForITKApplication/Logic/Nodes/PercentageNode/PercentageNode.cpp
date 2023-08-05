#include "PercentageNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

PercentageNode::PercentageNode(UniqueIDProvider *idProvider, std::string_view name) : Node{idProvider, name}
{
    outputPins.emplace_back(std::make_unique<PercentagePin>(idProvider, this));
    percentagePin = outputPins.back().get();
}

void PercentageNode::populateOutputPins()
{
    percentagePin->payload = percentage;
}

json PercentageNode::serialize()
{
    json serializedPercentageNode          = Node::serialize();
    serializedPercentageNode["percentage"] = percentage;

    return serializedPercentageNode;
}

void PercentageNode::deserialize(json data)
{
    Node::deserialize(data);
    percentage = data["percentage"];

    percentagePin = outputPins.back().get();
}

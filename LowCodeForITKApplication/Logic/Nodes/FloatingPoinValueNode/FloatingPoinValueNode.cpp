#include "FloatingPoinValueNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

FloatingPoinValueNode::FloatingPoinValueNode(UniqueIDProvider *idProvider) : Node{idProvider, typeName}
{
    outputPins.emplace_back(std::make_unique<FloatingPointValuePin>(idProvider, this));
    valuePin = outputPins.back().get();
}

void FloatingPoinValueNode::populateOutputPins()
{
    valuePin->payload = value;
}

json FloatingPoinValueNode::serialize()
{
    json serialzedNode     = Node::serialize();
    serialzedNode["value"] = value;

    return serialzedNode;
}

void FloatingPoinValueNode::deserialize(json data)
{
    Node::deserialize(data);
    value    = data["value"];
    valuePin = outputPins.at(0).get();
}

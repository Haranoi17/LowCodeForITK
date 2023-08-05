#include "FloatingPointValueNode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

FloatingPointValueNode::FloatingPointValueNode(UniqueIDProvider *idProvider) : Node{idProvider, typeName}
{
    outputPins.emplace_back(std::make_unique<FloatingPointValuePin>(idProvider, this));
    valuePin = outputPins.back().get();
}

void FloatingPointValueNode::populateOutputPins()
{
    valuePin->payload = value;
}

json FloatingPointValueNode::serialize()
{
    json serialzedNode     = Node::serialize();
    serialzedNode["value"] = value;

    return serialzedNode;
}

void FloatingPointValueNode::deserialize(json data)
{
    Node::deserialize(data);
    value    = data["value"];
    valuePin = outputPins.at(0).get();
}

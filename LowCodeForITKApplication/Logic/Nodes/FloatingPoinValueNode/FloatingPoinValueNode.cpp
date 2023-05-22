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

void FloatingPoinValueNode::deserialize(json data)
{
    Node::deserialize(data);
    valuePin = outputPins.at(0).get();
}

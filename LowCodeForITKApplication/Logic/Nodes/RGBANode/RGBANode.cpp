#include "RGBANode.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

RGBANode::RGBANode(UniqueIDProvider *idProvider) : Node{idProvider, typeName}
{
    outputPins.emplace_back(std::make_unique<RGBAPin>(idProvider, this));
    rgbaPin = outputPins.back().get();
}

void RGBANode::populateOutputPins()
{
    rgbaPin->payload = rgbaValues;
}

json RGBANode::serialize()
{
    json serializedRGBANode     = Node::serialize();
    serializedRGBANode["red"]   = rgbaValues.GetRed();
    serializedRGBANode["green"] = rgbaValues.GetGreen();
    serializedRGBANode["blue"]  = rgbaValues.GetBlue();
    serializedRGBANode["alpha"] = rgbaValues.GetAlpha();

    return serializedRGBANode;
}

void RGBANode::deserialize(json data)
{
    Node::deserialize(data);
    rgbaValues.SetRed(data["red"]);
    rgbaValues.SetGreen(data["green"]);
    rgbaValues.SetBlue(data["blue"]);
    rgbaValues.SetAlpha(data["alpha"]);

    rgbaPin = outputPins.back().get();
}

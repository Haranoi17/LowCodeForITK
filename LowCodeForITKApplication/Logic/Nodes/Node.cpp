#include "Node.hpp"
#include <Logic/Pins/DefinedPins/DefinedPins.hpp>
#include <algorithm>

void Node::calculate()
{
    dirty = true;
}

void Node::populateOutputPins()
{
}

void Node::setPosition(Position pos)
{
    position = pos;
}

Position Node::getPosition() const
{
    return position;
}

json Node::serialize()
{
    json serializedNode;

    serializedNode["id"]   = id;
    serializedNode["name"] = typeName;
    serializedNode["posX"] = position.x;
    serializedNode["posY"] = position.y;

    std::ranges::for_each(inputPins, [&](const auto &pin) { serializedNode["inputPins"].push_back(pin->serialize()); });
    std::ranges::for_each(outputPins,
                          [&](const auto &pin) { serializedNode["outputPins"].push_back(pin->serialize()); });

    return serializedNode;
}

void Node::deserialize(json data)
{
    id       = data["id"];
    typeName = data["name"];
    position = Position{data["posX"], data["posY"]};

    inputPins  = deserializePins(data["inputPins"]);
    outputPins = deserializePins(data["outputPins"]);
}

std::vector<std::unique_ptr<Pin>> Node::deserializePins(json pinsData)
{
    std::vector<std::unique_ptr<Pin>> pins;
    for (auto pinData : pinsData)
    {
        std::string pinType   = pinData["typeName"];
        auto        pinObject = typeNameToPinObject.at(pinType)();
        pinObject->deserialize(pinData);

        pins.emplace_back(std::move(pinObject));
    }

    return pins;
}
#include "Node.hpp"
#include <Pins/DefinedPins/DefinedPins.hpp>
#include <algorithm>

void Node::calculate()
{
    std::cout << "NODE:\n   " << name << " node calculation\n";
    dirty = true;
}

void Node::populateOutputPins()
{
    std::cout << "  " << name << " populating output pins \n";
}

json Node::serialize()
{
    json serializedNode;

    serializedNode["id"]   = id;
    serializedNode["name"] = name;
    std::ranges::for_each(m_inputPins,
                          [&](const auto &pin) { serializedNode["inputPins"].push_back(pin->serialize()); });
    std::ranges::for_each(m_outputPins,
                          [&](const auto &pin) { serializedNode["outputPins"].push_back(pin->serialize()); });

    return serializedNode;
}

void Node::deserialize(json data)
{
    id   = data["id"];
    name = data["name"];

    m_inputPins  = deserializePins(data["inputPins"]);
    m_outputPins = deserializePins(data["outputPins"]);
}

std::vector<std::unique_ptr<Pin>> Node::deserializePins(json pinsData)
{
    std::vector<std::unique_ptr<Pin>> pins;
    for (auto pinData : pinsData)
    {
        std::string pinType   = pinData["name"];
        auto        pinObject = typeNameToPinObject.at(pinType)();
        pinObject->deserialize(pinData);

        pins.emplace_back(std::move(pinObject));
    }

    return pins;
}
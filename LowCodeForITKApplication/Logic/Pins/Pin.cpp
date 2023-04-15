#include "Logic/Pins/Pin.hpp"

void Pin::calculate()
{
}

json Pin::serialize()
{
    json serializedPin;
    serializedPin["id"]       = id;
    serializedPin["typeName"] = typeName;
    return serializedPin;
}

void Pin::deserialize(json data)
{
    id       = data["id"];
    typeName = data["typeName"];
}

std::string Pin::getDrawText() const
{
    return contextName.has_value() ? std::format("{}<{}>", contextName.value(), typeName) : typeName;
}

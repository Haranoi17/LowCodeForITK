#include "Logic/Pins/Pin.hpp"

Pin::~Pin()
{
    if (idProvider)
    {
        idProvider->freeID(id);
    }
}

void Pin::calculate()
{
}

json Pin::serialize()
{
    json serializedPin;
    serializedPin["id"]          = id;
    serializedPin["typeName"]    = typeName;
    serializedPin["contextName"] = contextName.value_or("none");
    return serializedPin;
}

void Pin::deserialize(json data)
{
    id          = data["id"];
    typeName    = data["typeName"];
    contextName = data["contextName"] == "none" ? std::nullopt : std::make_optional(data["contextName"]);
}

std::string Pin::getDrawText() const
{
    return contextName.has_value() ? std::format("{}<{}>", contextName.value(), typeName) : typeName;
}

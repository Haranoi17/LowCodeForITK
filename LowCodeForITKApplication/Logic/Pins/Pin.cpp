#include "Logic/Pins/Pin.hpp"

json Pin::serialize()
{
    json serializedPin;
    serializedPin["id"]   = id;
    serializedPin["name"] = name;
    return serializedPin;
}

void Pin::deserialize(json data)
{
    id   = data["id"];
    name = data["name"];
}

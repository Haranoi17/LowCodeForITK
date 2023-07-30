#include "Link.hpp"

Link::Link(UniqueIDProvider *idProvider, std::pair<IDType, IDType> pins) : idProvider{idProvider}, id{idProvider->generateID()}, pinIds{pins}
{
}

Link::~Link()
{
    if (idProvider)
    {
        idProvider->freeID(id);
    }
}

json Link::serialize()
{
    json serializedLink;
    serializedLink["id"]     = id;
    serializedLink["pinIds"] = pinIds;
    return serializedLink;
}

void Link::deserialize(json data)
{
    id     = data["id"];
    pinIds = data["pinIds"];
}

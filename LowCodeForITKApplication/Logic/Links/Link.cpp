#include "Link.hpp"

LinkInfo::LinkInfo(UniqueIDProvider *idProvider, std::pair<IDType, IDType> pins)
    : idProvider{idProvider}, id{idProvider->generateID()}, pinIds{pins}
{
}

json LinkInfo::serialize()
{
    json serializedLinkInfo;
    serializedLinkInfo["id"]     = id;
    serializedLinkInfo["pinIds"] = pinIds;
    return serializedLinkInfo;
}

void LinkInfo::deserialize(json data)
{
    id     = data["id"];
    pinIds = data["pinIds"];
}

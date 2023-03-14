#include "Link.hpp"

LinkInfo::LinkInfo(IDType id, std::pair<IDType, IDType> pins) : id{id}, pinIds{pins}
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

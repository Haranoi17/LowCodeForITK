#pragma once
#include "Logic/Interfaces/Identifiable.hpp"
#include "Logic/Interfaces/Serializeable.hpp"
#include "Logic/UniqueIDProvider/UniqueIDProvider.hpp"
#include <utility>

class LinkInfo : public Serializable
{
  public:
    LinkInfo(UniqueIDProvider *idProvider, std::pair<IDType, IDType> pins);
    LinkInfo() = default;

    ~LinkInfo()
    {
        if (idProvider)
        {
            idProvider->freeID(id);
        }
    }

    json serialize() override;
    void deserialize(json data) override;

    UniqueIDProvider         *idProvider;
    IDType                    id;
    std::pair<IDType, IDType> pinIds;
};
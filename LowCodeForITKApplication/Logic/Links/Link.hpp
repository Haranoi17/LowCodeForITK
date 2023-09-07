#pragma once
#include "Logic/Interfaces/Identifiable.hpp"
#include "Logic/Interfaces/Serializeable.hpp"
#include "Logic/UniqueIDProvider/UniqueIDProvider.hpp"
#include <utility>

class Link : public Serializable
{
  public:
    Link(UniqueIDProvider *idProvider, std::pair<IDType, IDType> pins);
    Link(UniqueIDProvider *idProvider);
    Link() = default;

    ~Link();

    json serialize() override;
    void deserialize(json data) override;

    UniqueIDProvider         *idProvider;
    IDType                    id;
    std::pair<IDType, IDType> pinIds;
};
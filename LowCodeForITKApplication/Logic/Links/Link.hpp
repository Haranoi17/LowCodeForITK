#pragma once
#include "Logic/Interfaces/Identifiable.hpp"
#include "Logic/Interfaces/Serializeable.hpp"
#include <utility>

class LinkInfo : public Serializable
{
  public:
    LinkInfo(IDType id, std::pair<IDType, IDType> pins);
    LinkInfo() = default;

    json serialize() override;
    void deserialize(json data) override;

    IDType                    id;
    std::pair<IDType, IDType> pinIds;
};
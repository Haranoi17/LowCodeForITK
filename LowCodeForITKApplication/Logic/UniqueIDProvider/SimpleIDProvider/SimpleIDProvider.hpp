#pragma once
#include "Logic/UniqueIDProvider/UniqueIDProvider.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
#include <stack>
#include <vector>
class SimpleIDProvider final : public UniqueIDProvider
{
  public:
    ~SimpleIDProvider() = default;

    json serialize() override
    {
        json serializedIdProvider;
        serializedIdProvider["lastId"] = m_lastId;

        if (freeIds.size())
        {
            IDType             *end   = &freeIds.top() + 1;
            IDType             *begin = end - freeIds.size();
            std::vector<IDType> freeIdsAsVector(begin, end);
            for (auto id : freeIdsAsVector)
            {
                serializedIdProvider["freeIds"].push_back(id);
            }
        }

        return serializedIdProvider;
    }

    void deserialize(json data) override
    {
        m_lastId = data["lastId"];

        for (auto id : data["freeIds"])
        {
            freeIds.push(id);
        }
    }

    IDType generateID() override;
    void   freeID(IDType id) override;

  private:
    IDType             m_lastId{0};
    std::stack<IDType> freeIds;
};
#include "SimpleIDProvider.hpp"
#include <algorithm>

IDType SimpleIDProvider::generateID()
{
    if (!freeIds.empty())
    {
        auto Id = freeIds.top();
        freeIds.pop();
        return Id;
    }

    return ++m_lastId;
}

void SimpleIDProvider::freeID(IDType id)
{
    freeIds.push(id);
}

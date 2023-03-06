#pragma once
#include <Serializeable.hpp>

using IDType = unsigned;

class UniqueIDProvider : public Serializable
{
  public:
    virtual ~UniqueIDProvider()      = default;
    virtual IDType generateID()      = 0;
    virtual void   freeID(IDType id) = 0;
};
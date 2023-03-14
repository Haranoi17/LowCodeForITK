#pragma once
#include <Serializeable.hpp>

class UniqueIDProvider : public Serializable
{
  public:
    virtual ~UniqueIDProvider()      = default;
    virtual IDType generateID()      = 0;
    virtual void   freeID(IDType id) = 0;
};
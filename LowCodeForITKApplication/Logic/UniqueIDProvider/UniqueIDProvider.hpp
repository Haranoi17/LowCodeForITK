#pragma once
#include "Logic/Interfaces/Identifiable.hpp"
#include "Logic/Interfaces/Serializeable.hpp"

class UniqueIDProvider : public Serializable
{
  public:
    virtual ~UniqueIDProvider()      = default;
    virtual IDType generateID()      = 0;
    virtual void   freeID(IDType id) = 0;
};
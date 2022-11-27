#pragma once

using IDType = unsigned;

class UniqueIDProvider
{
  public:
    virtual ~UniqueIDProvider()      = default;
    virtual IDType generateID()      = 0;
    virtual void   freeID(IDType id) = 0;
};
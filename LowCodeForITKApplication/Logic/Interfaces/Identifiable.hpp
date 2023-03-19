#pragma once

using IDType = unsigned;

class Identifiable
{
  public:
    virtual IDType getID() const = 0;
};
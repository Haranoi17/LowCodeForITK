#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Serializable
{
  public:
    virtual ~Serializable()                     = default;
    
    virtual json serialize()            = 0;
    virtual void deserialize(json data) = 0;
};
#pragma once
#include "Nodes\Interface\Node.hpp"
#include "Pins\Interface\Pin.hpp"

struct DrawStrategy
{
    virtual ~DrawStrategy() = default;
    virtual void draw()     = 0;
};
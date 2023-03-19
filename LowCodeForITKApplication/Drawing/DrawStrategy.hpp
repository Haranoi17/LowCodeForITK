#pragma once

struct DrawStrategy
{
    virtual ~DrawStrategy() = default;
    virtual void draw()     = 0;
};
#pragma once

struct Position
{
    float x;
    float y;
};

class Positionable
{
  public:
    virtual void     setPosition(Position position) = 0;
    virtual Position getPosition() const            = 0;
};
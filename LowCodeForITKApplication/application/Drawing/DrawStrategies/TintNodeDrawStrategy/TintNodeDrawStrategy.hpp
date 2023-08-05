#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/TintNode/TintNode.hpp"

struct TintNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
  using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

  private:
    TintNode *tintNode{dynamic_cast<TintNode *>(node)};
};
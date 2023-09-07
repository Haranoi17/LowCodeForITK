#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/GaussianBlurNode/GaussianBlurNode.hpp"

struct GaussianBlurNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

  private:
    GaussianBlurNode *gaussianBlurNode{dynamic_cast<GaussianBlurNode *>(node)};
};


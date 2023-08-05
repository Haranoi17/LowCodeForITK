#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/BinaryThresholdNode/BinaryThresholdNode.hpp"

struct BinaryThresholdNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

  private:
    BinaryThresholdNode *binaryThresholdNode{dynamic_cast<BinaryThresholdNode *>(node)};
};
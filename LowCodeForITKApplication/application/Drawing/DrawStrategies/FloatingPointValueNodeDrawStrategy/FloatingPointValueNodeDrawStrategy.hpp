#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/FloatingPointValueNode/FloatingPointValueNode.hpp"

struct FloatingPointValueNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    FloatingPointValueNode *floatingPoinValueNode{dynamic_cast<FloatingPointValueNode *>(node)};
};
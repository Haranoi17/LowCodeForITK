#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/PercentageNode/PercentageNode.hpp"

struct PercentageNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    PercentageNode *percentageNode{dynamic_cast<PercentageNode *>(node)};
};
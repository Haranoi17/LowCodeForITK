#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/RGBANode/RGBANode.hpp"

struct RGBANodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    RGBANode *rgbaNode{dynamic_cast<RGBANode *>(node)};
};
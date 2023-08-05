#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/SampleNodeForAutomation/SampleNodeForAutomation.hpp"

struct SampleNodeDrawStrategyForAutomation : public BlueprintNodeDrawStrategy
{
  using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

  private:
    SampleNodeForAutomation *SampleNodeForAutomationParam{dynamic_cast<SampleNodeForAutomation *>(node)};
};
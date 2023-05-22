#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/SampleNodeForAutomation/SampleNodeForAutomation.hpp"

#include "Application/TexturesOperationsProxySingleton.hpp"

struct SampleNodeDrawStrategyForAutomation : public BlueprintNodeDrawStrategy
{
    SampleNodeDrawStrategyForAutomation(SampleNodeForAutomation *SampleNodeForAutomationParam);

  private:
    SampleNodeForAutomation *SampleNodeForAutomationParam;
};
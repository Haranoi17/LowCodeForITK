#include "SampleNodeDrawStrategyForAutomation.hpp"
#include "Application/LowCodeForITKApplication.hpp"

SampleNodeDrawStrategyForAutomation::SampleNodeDrawStrategyForAutomation(SampleNodeForAutomation *SampleNodeForAutomationParam)
    : BlueprintNodeDrawStrategy{SampleNodeForAutomationParam}, SampleNodeForAutomationParam{SampleNodeForAutomationParam}
{
}

#include "PercentageNodeDrawStrategy.hpp"

void PercentageNodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    auto prevValue = percentageNode->percentage;

    ImGui::SliderFloat("%", &percentageNode->percentage, 0.0f, 1.0f);

    percentageNode->dirty = prevValue != percentageNode->percentage;
}

#include "FloatingPointValueNodeDrawStrategy.hpp"

void FloatingPointValueNodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    const auto previousValue = floatingPoinValueNode->value;
    ImGui::InputFloat("value", &floatingPoinValueNode->value, 0.1);
    node->dirty = previousValue != floatingPoinValueNode->value;
}

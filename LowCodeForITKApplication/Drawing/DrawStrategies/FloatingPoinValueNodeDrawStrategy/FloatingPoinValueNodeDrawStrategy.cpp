#include "FloatingPoinValueNodeDrawStrategy.hpp"
#include "Application/LowCodeForITKApplication.hpp"

FloatingPoinValueNodeDrawStrategy::FloatingPoinValueNodeDrawStrategy(FloatingPoinValueNode *floatingPoinValueNode)
    : BlueprintNodeDrawStrategy{floatingPoinValueNode}, floatingPoinValueNode{floatingPoinValueNode}
{
}

void FloatingPoinValueNodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    const auto previousValue = floatingPoinValueNode->value;
    ImGui::InputFloat("value", &floatingPoinValueNode->value, 0.1);
    node->dirty = previousValue != floatingPoinValueNode->value;
}

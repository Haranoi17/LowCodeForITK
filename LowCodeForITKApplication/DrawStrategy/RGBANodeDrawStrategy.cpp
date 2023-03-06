#include "RGBANodeDrawStrategy.hpp"

#include "DrawDefaults.hpp"
#include "imgui.h"
#include <limits>
#include <map>

RGBANodeDrawStrategy::RGBANodeDrawStrategy(RGBANode *rgbaNode)
    : BlueprintNodeDrawStrategy{rgbaNode}, m_rgbaNode{rgbaNode}
{
}

void RGBANodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    static const std::map<int, std::string> dimentionNameMap{{0, "r"}, {1, "g"}, {2, "b"}, {3, "a"}};
    static constexpr int                    min = std::numeric_limits<RGBANode::PixelType::ComponentType>::min();
    static constexpr int                    max = std::numeric_limits<RGBANode::PixelType::ComponentType>::max();

    for (auto i = 0; i < m_rgbaNode->rgbaValues.Dimension; i++)
    {
        auto currentValue = static_cast<int>(m_rgbaNode->rgbaValues.GetElement(i));
        ImGui::SliderInt(dimentionNameMap.at(i).c_str(), &currentValue, min, max);
        m_rgbaNode->rgbaValues.SetElement(i, static_cast<RGBANode::PixelType::ComponentType>(currentValue));
    }
}
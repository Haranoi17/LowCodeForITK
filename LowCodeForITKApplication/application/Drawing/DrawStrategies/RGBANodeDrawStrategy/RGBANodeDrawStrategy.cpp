#include "RGBANodeDrawStrategy.hpp"

#include "Application/Drawing/DrawDefaults.hpp"
#include "imgui.h"
#include <limits>
#include <map>

void RGBANodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    static const std::map<int, std::string> dimentionNameMap{{0, "r"}, {1, "g"}, {2, "b"}, {3, "a"}};
    static constexpr int                    min = std::numeric_limits<RGBANode::PixelType::ComponentType>::min();
    static constexpr int                    max = std::numeric_limits<RGBANode::PixelType::ComponentType>::max();

    auto prevValue = rgbaNode->rgbaValues;
    for (auto i = 0; i < rgbaNode->rgbaValues.Dimension; i++)
    {
        auto currentValue = static_cast<int>(rgbaNode->rgbaValues.GetElement(i));
        ImGui::SliderInt(dimentionNameMap.at(i).c_str(), &currentValue, min, max);
        rgbaNode->rgbaValues.SetElement(i, static_cast<RGBANode::PixelType::ComponentType>(currentValue));

        if (prevValue[i] != rgbaNode->rgbaValues[i])
        {
            rgbaNode->dirty = true;
        }
    }
}
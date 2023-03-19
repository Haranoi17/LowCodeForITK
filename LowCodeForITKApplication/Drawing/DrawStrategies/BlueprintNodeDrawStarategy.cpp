#include "BlueprintNodeDrawStarategy.hpp"

#include "Drawing/DrawDefaults.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include "imgui_node_editor.h"
#include <format>

#include "Application/TexturesAccessorSingleton.hpp"
#include "Drawing/blueprints/builders.h"
#include "Drawing/blueprints/widgets.h"

namespace ed   = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy(Node *testNode) : NodeDrawStrategy{testNode}
{
}

void BlueprintNodeDrawStrategy::draw()
{
    auto texturesOperations = TexturesOperationsProxySingleton::instance();
    auto texturesAccessor   = TexturesAccessorSingleton::instance();

    auto nodeHeaderTexture = texturesAccessor->getNodeHeaderTexture();

    nodeBuilder = util::BlueprintNodeBuilder(nodeHeaderTexture,
                                             texturesOperations->GetTextureWidth(nodeHeaderTexture),
                                             texturesOperations->GetTextureHeight(nodeHeaderTexture));

    setStyleVariables();

    nodeBuilder.Begin(node->id);

    pushImguiVariables();

    drawHeader();

    drawInputPins();

    nodeBuilder.Middle();

    drawOutputPins();

    nodeSpecificFunctionalitiesBeforeNodeEnd();

    nodeBuilder.End();

    nodeSpecificFunctionalitiesAfterNodeEnd();

    unsetStyleVariables();
}

void BlueprintNodeDrawStrategy::setStyleVariables()
{
    auto alpha = ImGui::GetStyle().Alpha;
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
    ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(20, 20, 20, 150));
    ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(125, 125, 125, 200));
    ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(229, 229, 229, 60));
    ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(125, 125, 125, 60));
    ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
    ed::PushStyleVar(ed::StyleVar_NodeRounding, 2);
    ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
    ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
    ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
    ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
    ed::PushStyleVar(ed::StyleVar_PinRadius, 6.0f);
}

void BlueprintNodeDrawStrategy::unsetStyleVariables()
{
    ImGui::PopStyleVar();
    ed::PopStyleColor(4);
    ed::PopStyleVar(7);
}

void BlueprintNodeDrawStrategy::drawHeader()
{
    nodeBuilder.Header(ImColor{0, 0, 255});
    ImGui::Spring(0);
    ImGui::TextUnformatted(node->name.c_str());
    ImGui::Spring(1);
    ImGui::Dummy(ImVec2(0, 28));
    nodeBuilder.EndHeader();
}

void BlueprintNodeDrawStrategy::drawInputPins()
{
    DEBUG_RECTANGLE();
    if (const auto noInputPins = !node->inputPins.size(); noInputPins)
    {
        auto size = ImGui::GetItemRectSize();
        ImGui::Dummy(ImVec2{size.x / 2.0f, 0.0f});
    }
    for (const auto &inputPin : node->inputPins)
    {
        auto isConnected = !inputPin->connectedPins.empty();
        nodeBuilder.Input(inputPin->id);

        ax::Widgets::Icon(ImVec2(g_pinIconSize, g_pinIconSize),
                          ax::Widgets::IconType::Circle,
                          isConnected,
                          g_pinIconColor,
                          g_pinIconInnerColor);

        ImGui::Spring(1);

        ImGui::TextUnformatted(inputPin->name.c_str());
        ImGui::Spring(1);

        nodeBuilder.EndInput();
    }
}

void BlueprintNodeDrawStrategy::drawOutputPins()
{
    for (auto &outputPin : node->outputPins)
    {
        auto isConnected = !outputPin->connectedPins.empty();

        nodeBuilder.Output(outputPin->id);

        ImGui::Spring(0);
        ImGui::TextUnformatted(outputPin->name.c_str());
        ImGui::Spring(0);

        ax::Widgets::Icon(ImVec2(g_pinIconSize, g_pinIconSize),
                          ax::Widgets::IconType::Circle,
                          isConnected,
                          g_pinIconColor,
                          g_pinIconInnerColor);

        nodeBuilder.EndOutput();
    }
}

void BlueprintNodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
}

void BlueprintNodeDrawStrategy::nodeSpecificFunctionalitiesAfterNodeEnd()
{
}

void BlueprintNodeDrawStrategy::pushImguiVariables()
{
    ImGui::PushItemWidth(200);
}
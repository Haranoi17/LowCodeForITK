#include "LowCodeForITKApplication.hpp"
#include "TexturesOperationsProxySingleton.hpp"
#include <algorithm>

void LowCodeForITKApplication::OnStart()
{
    ed::Config config;
    config.SettingsFile = "LowCodeForITKApplication.json";
    m_Context           = ed::CreateEditor(&config);

    TexturesOperationsProxySingleton::instance(this); // set this application as proxys operator

    m_logic.init();
}

void LowCodeForITKApplication::OnStop()
{
    ed::DestroyEditor(m_Context);
}

void LowCodeForITKApplication::buttonForTriggeringEvaluation()
{
    if (ImGui::Button("TriggerEvaluationButton", ImVec2{200, 40}))
    {
        m_logic.propagateEvaluationThroughTheNodes();
    }
}

void LowCodeForITKApplication::OnFrame(float deltaTime)
{
    showFPS();
    ImGui::SameLine();
    ImGui::Separator();

    ed::SetCurrentEditor(m_Context);

    ed::Begin("Editor", ImVec2(0.0, 0.0f));

    drawMenu();

    auto cursorTopLeft = ImGui::GetCursorScreenPos();

    std::ranges::for_each(m_logic.getNodesDrawStrategies(),
                          [&](const std::unique_ptr<NodeDrawStrategy> &nodeDrawStrategy) { nodeDrawStrategy->draw(); });

    drawingLinks();

    pinsVisualLinking();

    handleDeleting();
    ImGui::SetCursorScreenPos(cursorTopLeft);

    ed::End(); // Editor

    if (m_FirstFrame)
        ed::NavigateToContent(0.0f);

    ed::SetCurrentEditor(nullptr);

    m_FirstFrame = false;
}

void LowCodeForITKApplication::drawMenu()
{
    ed::Suspend();
    ImGui::Begin("menu");
    buttonForTriggeringEvaluation();
    buttonForNodesModal();

    ImGuiDemoModal();

    ImGui::End();
    ed::Resume();
}

void LowCodeForITKApplication::ImGuiDemoModal()
{
    static bool open{false};
    if (ImGui::Button("demo"))
    {
        open = true;
    }

    if (open)
    {
        ImGui::ShowDemoWindow(&open);
    }
}

void LowCodeForITKApplication::buttonForNodesModal()
{
    if (ImGui::Button("Nodes Drawer"))
    {
        ImGui::OpenPopup("NodesDrawerModal");
    }

    if (ImGui::BeginPopupModal("NodesDrawerModal"))
    {

        for (auto [nodeName, nodeWithDrawStrategyCreator] : m_logic.m_nodeCreators)
        {
            if (ImGui::Button(nodeName.c_str()))
            {
                auto nodeWithDrawStrategy = nodeWithDrawStrategyCreator();
                m_logic.addNode(std::move(nodeWithDrawStrategy->node));
                m_logic.addNodeDrawStrategy(std::move(nodeWithDrawStrategy->drawStrategy));
                ImGui::CloseCurrentPopup();
            }
        }

        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void LowCodeForITKApplication::handleDeleting()
{
    if (ed::BeginDelete())
    {
        linksDeletion();
        ed::NodeId deletedNodeId;
        while (ed::QueryDeletedNode(&deletedNodeId))
        {
            if (ed::AcceptDeletedItem())
            {
                m_logic.deleteNode(deletedNodeId.Get());
            }
        }
    }
    ed::EndDelete();
}

void LowCodeForITKApplication::linksDeletion()
{
    ed::LinkId deletedLinkId;
    while (ed::QueryDeletedLink(&deletedLinkId))
    {
        if (ed::AcceptDeletedItem())
        {
            m_logic.deleteLink(deletedLinkId.Get());
        }
    }
}

void LowCodeForITKApplication::showFPS()
{
    auto &io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
}

void LowCodeForITKApplication::drawingLinks()
{
    for (auto &linkInfo : m_logic.getLinks())
        ed::Link(linkInfo->id, linkInfo->pinIds.first, linkInfo->pinIds.second);
}

void LowCodeForITKApplication::pinsVisualLinking()
{
    if (ed::BeginCreate())
    {
        ed::PinId inputPinId, outputPinId;
        if (ed::QueryNewLink(&inputPinId, &outputPinId))
        {
            if (m_logic.isLinkPossible(std::make_pair(inputPinId.Get(), outputPinId.Get())))
            {
                if (ed::AcceptNewItem())
                {
                    m_logic.createLink(std::make_pair(inputPinId.Get(), outputPinId.Get()));
                }
            }
            else
            {
                ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
            }
        }
    }
    ed::EndCreate(); // Wraps up object creation action handling.
}

#include "LowCodeForITKApplication.hpp"
#include "DrawVisitor/ImguiNodeEditorDrawVisitor.hpp"
#include <algorithm>

void LowCodeForITKApplication::OnStart()
{
    ed::Config config;
    config.SettingsFile = "LowCodeForITKApplication.json";
    m_Context           = ed::CreateEditor(&config);

    m_drawVisitor = std::make_unique<ImguiNodeEditorDrawVisitor>(this);
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
    buttonForTriggeringEvaluation();
    ImGui::SameLine();
    buttonForNodesModal();

    ImGui::Separator();

    ed::SetCurrentEditor(m_Context);

    ed::Begin("Editor", ImVec2(0.0, 0.0f));
    auto cursorTopLeft = ImGui::GetCursorScreenPos();

    std::ranges::for_each(m_logic.getNodes(),
                          [&](const std::unique_ptr<Node> &node) { node->acceptDrawVisitor(m_drawVisitor.get()); });

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

void LowCodeForITKApplication::buttonForNodesModal()
{
    if (ImGui::Button("Nodes Drawer"))
    {
        ImGui::OpenPopup("NodesDrawerModal");
    }

    if (ImGui::BeginPopupModal("NodesDrawerModal"))
    {

        for (auto nameCreatorPair : m_logic.m_nodeCreators)
        {
            auto nodeName    = nameCreatorPair.first;
            auto nodeCreator = nameCreatorPair.second;
            if (ImGui::Button(nodeName.c_str()))
            {
                m_logic.addNode(nodeCreator());
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
        ed::LinkId deletedLinkId;
        while (ed::QueryDeletedLink(&deletedLinkId))
        {
            if (ed::AcceptDeletedItem())
            {
                m_logic.deleteLink(deletedLinkId.Get());
            }
        }
    }
    ed::EndDelete();
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

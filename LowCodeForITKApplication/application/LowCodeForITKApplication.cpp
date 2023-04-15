#include "LowCodeForITKApplication.hpp"
#include "Logic/Interfaces/Identifiable.hpp"
#include "TexturesOperationsProxySingleton.hpp"
#include <algorithm>
#include <fstream>

std::string serializationFileName = "serialization.json";

void LowCodeForITKApplication::serialize()
{
    std::ofstream fstream;
    fstream.open(serializationFileName);
    fstream << m_logic.serialize();
    fstream.close();
}

void LowCodeForITKApplication::deserialize()
{
    json json;

    std::ifstream fstream;
    fstream.open(serializationFileName);
    if (fstream.is_open())
    {
        fstream >> json;
        m_logic.deserialize(json);
        m_logic.updateCreators();
        m_logic.propagateEvaluationThroughTheNodes();
        fstream.close();
    }
}

void LowCodeForITKApplication::OnStart()
{
    m_logic.updateCreators();

    m_config.SettingsFile = settingsFile;

    m_Context = ed::CreateEditor(&m_config);

    TexturesOperationsProxySingleton::instance(this);
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

void LowCodeForITKApplication::ButtonForSaving()
{
    if (ImGui::Button("save", ImVec2{100, 20}))
    {
        serialize();
    }
}
void LowCodeForITKApplication::ButtonForLoading()
{
    if (ImGui::Button("load", ImVec2{100, 20}))
    {
        deserialize();
    }
}

void LowCodeForITKApplication::OnFrame(float deltaTime)
{
    TexturesOperationsProxySingleton::instance()->DestroyTextures();

    showFPS();
    ImGui::SameLine();
    ImGui::Separator();

    ed::SetCurrentEditor(m_Context);

    ed::Begin("Editor", ImVec2(0.0, 0.0f));

    drawMenu();

    auto cursorTopLeft = ImGui::GetCursorScreenPos();

    std::ranges::for_each(m_logic.getNodesDrawStrategies(),
                          [&](NodeDrawStrategy *nodeDrawStrategy) { nodeDrawStrategy->draw(); });

    if (m_logic.innerNodesStateChanged() && logicFinished)
    {
        logicFinished = false;
        std::thread t{[this]() {
            m_logic.propagateEvaluationThroughTheNodes();
            m_logic.removeDirtyFlagsFromNodes();
            logicFinished = true;
        }};

        t.detach();
    }

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
    ButtonForSaving();
    ButtonForLoading();

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

        for (auto [nodeType, createNodeWithDrawStrategy] : m_logic.m_nodeCreators)
        {
            if (ImGui::Button(nodeType.c_str()))
            {
                auto nodeWithDrawStrategy = createNodeWithDrawStrategy();
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
            IDType logicInputPinId  = static_cast<IDType>(inputPinId.Get());
            IDType logicOutputPinId = static_cast<IDType>(outputPinId.Get());
            if (m_logic.isLinkPossible(std::make_pair(logicInputPinId, logicOutputPinId)))
            {
                if (ed::AcceptNewItem())
                {
                    m_logic.createLink(std::make_pair(logicInputPinId, logicOutputPinId));
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

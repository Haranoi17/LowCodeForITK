#include "LowCodeForITKApplication.hpp"
#include "Logic/Interfaces/Identifiable.hpp"
#include "NodeDefines.hpp"
#include "TexturesOperationsProxySingleton.hpp"
#include <algorithm>
#include <fstream>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <ranges>

std::string nodesDrawerPopupName = "NodesDrawerPopup";
std::string inputFileModalName   = "InputFileNameModal";

std::string projectFileExtention = ".json";

void LowCodeForITKApplication::serialize(std::string_view fileName)
{
    std::ofstream fstream;
    fstream.open(fileName);
    fstream << logic.serialize();
    fstream.close();
}

void LowCodeForITKApplication::deserialize(std::string_view fileName)
{
    json json;

    std::ifstream fstream;
    fstream.open(fileName);
    if (fstream.is_open())
    {
        fstream >> json;
        logic.deserialize(json);
        logic.updateCreators();
        logic.propagateEvaluationThroughTheNodes();
        fstream.close();
    }
}

void LowCodeForITKApplication::OnStart()
{
    RegisterNodes();
    logic.updateCreators();

    config.SettingsFile = settingsFile;

    context = ed::CreateEditor(&config);

    TexturesOperationsProxySingleton::instance(this);
}

void LowCodeForITKApplication::OnStop()
{
    ed::DestroyEditor(context);
}

void LowCodeForITKApplication::buttonForTriggeringEvaluation()
{
    if (ImGui::Button("TriggerEvaluationButton", ImVec2{200, 40}))
    {
        logic.propagateEvaluationThroughTheNodes();
    }
}

void LowCodeForITKApplication::drawMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        SubMenuFile();
        SubMenuNodes();
        ImGui::EndMainMenuBar();
    }
}

void LowCodeForITKApplication::SubMenuFile()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("open"))
        {
            browser.SetTypeFilters({projectFileExtention});
            browser.Open();
            loadProject = true;
        }

        ImGui::Separator();

        if (ImGui::MenuItem("save"))
        {
            if (currentProjectFileName.empty())
            {
                inputFileName = true;
            }
            else
            {
                serialize(currentProjectFileName);
            }
        }

        if (ImGui::MenuItem("save as"))
        {
            inputFileName = true;
        }

        ImGui::EndMenu();
    }
}

void LowCodeForITKApplication::SubMenuNodes()
{
    if (ImGui::BeginMenu("Nodes"))
    {
        for (const auto [functionality, functionalityName] : functionalityNameMap)
        {
            if (ImGui::BeginMenu(functionalityName.c_str()))
            {
                for (const auto [nodeType, createNodeWithDrawStrategy] : logic.nodeCreators)
                {
                    if (nodeTypeFunctionalityMap.at(nodeType) != functionality)
                    {
                        continue;
                    }

                    if (ImGui::MenuItem(nodeType.c_str()))
                    {
                        addNode(createNodeWithDrawStrategy());
                    }
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndMenu();
    }
}

void LowCodeForITKApplication::OnFrame(float deltaTime)
{
    TexturesOperationsProxySingleton::instance()->DestroyTextures();

    ed::SetCurrentEditor(context);
    ed::Begin("Editor", ImVec2(0.0, 0.0f));

    ed::Suspend();
    drawMenu();
    SaveNewFileForm();
    ProjectLoading();
    ed::Resume();

    pinsVisualLinking();
    nodesPopup();

    std::ranges::for_each(logic.getNodesDrawStrategies(),
                          [&](NodeDrawStrategy *nodeDrawStrategy) { nodeDrawStrategy->draw(); });

    if (logic.innerNodesStateChanged() && logicFinished)
    {
        logicFinished = false;
        std::thread t{[this]() {
            logic.propagateEvaluationThroughTheNodes();
            logic.removeDirtyFlagsFromNodes();
            logicFinished = true;
        }};

        t.detach();
    }

    drawingLinks();

    handleDeleting();

    ed::End(); // Editor

    if (isFirstFrame)
        ed::NavigateToContent(0.0f);

    ed::SetCurrentEditor(nullptr);

    isFirstFrame = false;
}

void LowCodeForITKApplication::ProjectLoading()
{
    if (loadProject)
    {
        browser.Display();
        if (browser.HasSelected())
        {
            const auto fileName = browser.GetSelected().filename().string();
            if (fileName != currentProjectFileName)
            {
                deserialize(fileName);
                currentProjectFileName = fileName;
            }
        }
        loadProject = browser.IsOpened();
    }
}

void LowCodeForITKApplication::SaveNewFileForm()
{
    if (!inputFileName)
    {
        return;
    }

    auto clean = [&]() {
        inputFileName = false;
        ImGui::CloseCurrentPopup();
    };

    if (!ImGui::IsPopupOpen(inputFileModalName.c_str()))
    {
        ImGui::OpenPopup(inputFileModalName.c_str());
    }

    if (ImGui::BeginPopupModal(inputFileModalName.c_str()))
    {
        constexpr int size = 100;
        char          fileNameBuffor[100];
        strcpy(fileNameBuffor, newFileName.data());

        ImGui::InputText("fileName", fileNameBuffor, size);
        newFileName = fileNameBuffor;

        if (ImGui::Button("save"))
        {
            serialize(newFileName + projectFileExtention);
            clean();
        }

        if (ImGui::Button("cancel"))
        {
            clean();
        }

        ImGui::EndPopup();
    }
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

void LowCodeForITKApplication::nodesPopup()
{
    ed::Suspend();
    if (!ImGui::IsPopupOpen(nodesDrawerPopupName.c_str()))
    {
        pinInitializingNodeCreationID = std::nullopt;
    }

    if (ImGui::BeginPopup(nodesDrawerPopupName.c_str()))
    {
        SubMenuNodes();
        if (pinInitializingNodeCreationID.has_value())
        {
            if (ImGui::BeginMenu("CompatibleNodes"))
            {
                for (const auto [functionality, functionalityName] : functionalityNameMap)
                {
                    if (ImGui::BeginMenu(functionalityName.c_str()))
                    {
                        for (const auto [nodeType, createNodeWithDrawStrategy] : logic.nodeCreators)
                        {
                            if (nodeTypeFunctionalityMap.at(nodeType) != functionality)
                            {
                                continue;
                            }
                            auto   mockNode = createNodeWithDrawStrategy();
                            IDType otherPinId;
                            if (std::ranges::any_of(
                                    logic.isInputPin(pinInitializingNodeCreationID.value()) ? mockNode->node->outputPins
                                                                                            : mockNode->node->inputPins,
                                    [&](const std::unique_ptr<Pin> &pin) {
                                        auto isSame =
                                            logic.getPinType(pinInitializingNodeCreationID.value()) == pin->typeName;
                                        if (isSame)
                                        {
                                            otherPinId = pin->id;
                                        }
                                        return isSame;
                                    }))
                            {
                                if (ImGui::MenuItem(nodeType.c_str()))
                                {
                                    addNode(std::move(mockNode));
                                    auto pinIdsPair = std::make_pair(otherPinId, pinInitializingNodeCreationID.value());
                                    if (logic.isLinkPossible(pinIdsPair))
                                    {
                                        logic.createLink(pinIdsPair);
                                    }
                                }
                            }
                        }
                        ImGui::EndMenu();
                    }
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndPopup();
    }

    ed::Resume();
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
                logic.deleteNode(deletedNodeId.Get());
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
            logic.deleteLink(deletedLinkId.Get());
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
    for (auto &linkInfo : logic.getLinks())
        ed::Link(linkInfo->id, linkInfo->pinIds.first, linkInfo->pinIds.second);
}

void LowCodeForITKApplication::addNode(std::unique_ptr<NodeWithDrawStrategy> nodeWithDrawStrategy)
{
    auto mpos = ImGui::GetIO().MousePos;
    ed::SetNodePosition(nodeWithDrawStrategy->node->id, ed::ScreenToCanvas(mpos));

    logic.addNode(std::move(nodeWithDrawStrategy->node));
    logic.addNodeDrawStrategy(std::move(nodeWithDrawStrategy->drawStrategy));
}

void LowCodeForITKApplication::pinsVisualLinking()
{

    auto showLabel = [](const char *label, ImColor color) {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
        auto size = ImGui::CalcTextSize(label);

        auto padding = ImGui::GetStyle().FramePadding;
        auto spacing = ImGui::GetStyle().ItemSpacing;

        ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

        auto rectMin = ImGui::GetCursorScreenPos() - padding;
        auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

        auto drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
        ImGui::TextUnformatted(label);
    };

    if (ed::BeginCreate())
    {
        ed::PinId inputPinId, outputPinId;
        if (ed::QueryNewLink(&inputPinId, &outputPinId))
        {
            IDType logicInputPinId  = static_cast<IDType>(inputPinId.Get());
            IDType logicOutputPinId = static_cast<IDType>(outputPinId.Get());
            if (logic.isLinkPossible(std::make_pair(logicInputPinId, logicOutputPinId)))
            {
                if (ed::AcceptNewItem(ImColor(0, 255, 0), 2.0f))
                {
                    logic.createLink(std::make_pair(logicInputPinId, logicOutputPinId));
                }
            }
            else
            {
                ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
            }
        }

        if (ed::QueryNewNode(&inputPinId, ImColor(0, 255, 0), 2.0f))
        {
            showLabel("+ Create Node", ImColor(32, 45, 32, 180));

            if (ed::AcceptNewItem())
            {
                pinInitializingNodeCreationID = std::make_optional(static_cast<IDType>(inputPinId.Get()));
                ImGui::OpenPopup(nodesDrawerPopupName.c_str());
            }
        }
    }
    ed::EndCreate();
}

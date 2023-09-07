#include "LowCodeForITKApplication.hpp"
#include "Logic/Interfaces/Identifiable.hpp"
#include <algorithm>
#include <fstream>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Logic/utilities.hpp"
#include <imgui_internal.h>
#include <range/v3/all.hpp>

std::string nodesDrawerPopupName = "NodesDrawerPopup";
std::string inputFileModalName   = "InputFileNameModal";

std::string projectFileExtention = ".project";

void LowCodeForITKApplication::serialize(std::string_view fileName)
{
    std::ofstream fstream;
    fstream.open(fileName);
    fstream << logic.serialize();
    fstream.close();
}

void LowCodeForITKApplication::deserialize(std::string_view fileName)
{
    try
    {
        json json;

        std::ifstream fstream;
        fstream.open(fileName);
        if (fstream.is_open())
        {
            fstream >> json;
            logic.deserialize(json);
            ranges::for_each(logic.getNodes(), [this](auto node) { registerDrawStrategyForNode(node); });
            logic.propagateEvaluationThroughTheNodes();
            fstream.close();
        }
    }
    catch (std::exception e)
    {
        std::cout << std::format("Could not load {}", fileName.data());
    }
}

void LowCodeForITKApplication::OnStart()
{
    config.SettingsFile = settingsFile;

    context = ed::CreateEditor(&config);

    texturesRepository->loadTextures(textureOperator.get());
}

void LowCodeForITKApplication::OnStop()
{
    ed::DestroyEditor(context);
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

        if (!currentProjectFileName.empty()) 
        {
            if (ImGui::MenuItem("save"))
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
                for (const auto nodeType : nodesRepository.getRegisteredNodeTypeNames())
                {
                    if (nodesRepository.getMapOfNodeFunctionalities().at(nodeType) != functionality)
                    {
                        continue;
                    }

                    if (ImGui::MenuItem(nodeType.c_str()))
                    {
                        auto newNode = nodesRepository.getMapOfNodeCreators().at(nodeType)->createFullyFunctional(logic.getIdProvider());
                        addNode(std::move(newNode));
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
    // TexturesOperationsProxySingleton::instance()->DestroyTextures();

    ed::SetCurrentEditor(context);
    ed::Begin("Editor", ImVec2(0.0, 0.0f));

    ed::Suspend();
    drawMenu();
    SaveNewFileForm();
    ProjectLoading();
    ed::Resume();

    pinsVisualLinking();
    nodesPopup();

    std::ranges::for_each(logic.getNodes(), [&](Node *node) { drawNode(node->id); });

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
            currentProjectFileName = newFileName;
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
        dragPinID                  = std::nullopt;
        mousePosAtNodesPopupOpened = std::nullopt;
    }

    if (ImGui::BeginPopup(nodesDrawerPopupName.c_str()))
    {
        SubMenuNodes();

        if (dragPinID.has_value())
        {
            if (ImGui::BeginMenu("CompatibleNodes"))
            {
                auto isDragPinInput       = logic.isInputPin(dragPinID.value());
                auto dragPinType          = logic.getPinType(dragPinID.value());
                auto nonOwningMockedNodes = nodesRepository.getMockedNodes() | std::views::transform(ToNonOwningPointer());
                auto compatibleNodes      = nonOwningMockedNodes | ranges::view::filter([&](const Node *node) {
                                           const auto &pinsToCheck = isDragPinInput ? node->outputPins : node->inputPins;
                                           return ranges::contains(pinsToCheck, dragPinType, &Pin::typeName);
                                       }) |
                                       ranges::to_vector;

                for (const auto [functionality, functionalityName] : functionalityNameMap)
                {
                    auto nodesWithCurrentFunctionality = compatibleNodes | ranges::views::filter([&](const Node *node) {
                                                             return nodesRepository.getMapOfNodeFunctionalities().at(node->typeName) == functionality;
                                                         }) |
                                                         ranges::to_vector;

                    if (const auto shouldEnableMenu = !nodesWithCurrentFunctionality.empty();
                        ImGui::BeginMenu(functionalityName.c_str(), shouldEnableMenu))
                    {
                        for (const auto &currFunNode : nodesWithCurrentFunctionality)
                        {
                            auto nodeType = currFunNode->typeName;

                            if (ImGui::MenuItem(nodeType.c_str()))
                            {
                                auto newNode = nodesRepository.getMapOfNodeCreators().at(nodeType)->createFullyFunctional(logic.getIdProvider());

                                const auto &pinsToIterate = isDragPinInput ? newNode->outputPins : newNode->inputPins;

                                std::vector<std::pair<IDType, std::string>> pairs =
                                    pinsToIterate |
                                    ranges::views::transform([](const std::unique_ptr<Pin> &pin) { return std::make_pair(pin->id, pin->typeName); }) |
                                    ranges::to_vector;

                                auto result = ranges::find_if(pairs, [&](const auto &pair) { return pair.second == dragPinType; });

                                auto otherID = result->first;

                                addNode(std::move(newNode));
                                auto pinIdsPair = std::make_pair(otherID, dragPinID.value());
                                if (logic.isLinkPossible(pinIdsPair))
                                {
                                    logic.createLink(pinIdsPair);
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
        nodeDeletion();
    }
    ed::EndDelete();
}

void LowCodeForITKApplication::nodeDeletion()
{
    ed::NodeId deletedNodeId;
    while (ed::QueryDeletedNode(&deletedNodeId))
    {
        if (ed::AcceptDeletedItem())
        {
            unregisterDrawStrategyForNode(deletedNodeId.Get());
            logic.deleteNode(deletedNodeId.Get());
        }
    }
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
    for (auto &link : logic.getLinks())
        ed::Link(link->id, link->pinIds.first, link->pinIds.second);
}

void LowCodeForITKApplication::addNode(std::unique_ptr<Node> newNode)
{
    auto mpos = mousePosAtNodesPopupOpened.value_or(ImGui::GetIO().MousePos);
    auto canvasPos = ed::ScreenToCanvas(mpos);
    newNode->setPosition(Position{canvasPos.x, canvasPos.y});

    logic.addNode(std::move(newNode));
    registerDrawStrategyForNode(logic.getLastAddedNode()); 
}

void LowCodeForITKApplication::registerDrawStrategyForNode(Node *node)
{
    drawStrategies[node->id] =
        nodesRepository.getMapOfDrawStrategyCreators().at(node->typeName)(node, textureOperator.get(), texturesRepository.get(), &colorPicker);
}

void LowCodeForITKApplication::unregisterDrawStrategyForNode(IDType nodeID)
{
    drawStrategies.erase(nodeID);
}

void LowCodeForITKApplication::drawNode(IDType nodeId)
{
    drawStrategies[nodeId]->draw();
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
                mousePosAtNodesPopupOpened = ImGui::GetIO().MousePos;
                dragPinID                  = std::make_optional(static_cast<IDType>(inputPinId.Get()));
                ImGui::OpenPopup(nodesDrawerPopupName.c_str());
            }
        }
    }
    ed::EndCreate();
}
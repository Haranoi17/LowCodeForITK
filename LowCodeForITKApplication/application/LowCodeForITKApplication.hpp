#include "Logic/Logic.hpp"
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"
#include "Logic/UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"
#include "NodeDefines.hpp"
#include "imgui_node_editor.h"
#include <Application/ApplicationTextureOperator.hpp>
#include <Application/ApplicationTexturesRepository.hpp>
#include <Application/ColorPicker.hpp>
#include <application.h>
#include <atomic>
#include <imfilebrowser.h>
#include <memory>

namespace ed = ax::NodeEditor;

struct LowCodeForITKApplication : public Application
{
    using Application::Application;

    void OnStart() override;

    void OnStop() override;

    void serialize(std::string_view fileName);
    void deserialize(std::string_view fileName);

    void OnFrame(float deltaTime) override;

    void ProjectLoading();

    void SaveNewFileForm();

    void drawMenu();

    void SubMenuFile();
    void SubMenuNodes();

    void ImGuiDemoModal();

    void nodesPopup();
    void drawingLinks();
    void addNode(std::unique_ptr<Node> newNode);
    void registerDrawStrategyForNode(Node *node);
    void unregisterDrawStrategyForNode(IDType nodeID);
    void drawNode(IDType nodeId);
    void pinsVisualLinking();

    void showFPS();
    void handleDeleting();

    void nodeDeletion();

    void linksDeletion();

    ax::NodeEditor::EditorContext *context      = nullptr; // Editor context, required to trace a editor state.
    bool                           isFirstFrame = true;    // Flag set for first frame only, some action need
                                                           // to be executed once.

    ed::Config               config;
    inline static const auto settingsFile = "LowCodeForITKApplication.json";
    std::string              currentProjectFileName;

    ImGui::FileBrowser browser;
    bool               inputFileName{false};
    std::string        newFileName;

    bool                  loadProject{false};
    std::optional<IDType> dragPinID;
    std::optional<ImVec2> mousePosAtNodesPopupOpened;

    std::atomic<bool> logicFinished{true};

    ApplicationNodesRepository                      nodesRepository{};
    Logic                                           logic{&nodesRepository};
    std::map<IDType, std::unique_ptr<DrawStrategy>> drawStrategies;
    std::unique_ptr<TextureOperator>                textureOperator{std::make_unique<ApplicationTextureOperator>(this)};
    std::unique_ptr<TexturesRepository>             texturesRepository{std::make_unique<ApplicationTexturesRepository>()};
    ColorPicker                                     colorPicker{&nodesRepository};
};

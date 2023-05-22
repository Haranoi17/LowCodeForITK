#include "Logic/Logic.hpp"
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"
#include "Logic/UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"
#include "imgui_node_editor.h"
#include <application.h>
#include <imgui-filebrowser/imfilebrowser.h>

#include <atomic>
namespace ed = ax::NodeEditor;

struct LowCodeForITKApplication : public Application
{
    using Application::Application;

    void OnStart() override;

    void OnStop() override;

    void buttonForTriggeringEvaluation();

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
    void addNode(std::unique_ptr<NodeWithDrawStrategy> nodeWithDrawStrategy);
    void pinsVisualLinking();

    void showFPS();
    void handleDeleting();

    void linksDeletion();

    ax::NodeEditor::EditorContext *context      = nullptr; // Editor context, required to trace a editor state.
    bool                           isFirstFrame = true;    // Flag set for first frame only, some action need
                                                           // to be executed once.

    Logic                    logic;
    ed::Config               config;
    inline static const auto settingsFile = "LowCodeForITKApplication.json";
    std::string              currentProjectFileName;

    ImGui::FileBrowser browser;
    bool               inputFileName{false};
    std::string        newFileName;

    bool                  loadProject{false};
    std::optional<IDType> dragPinID;

    std::atomic<bool> logicFinished{true};
};

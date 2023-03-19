#include "Logic/Logic.hpp"
#include "Logic/Nodes/Node.hpp"
#include "Logic/Pins/Pin.hpp"
#include "Logic/UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"
#include "imgui_node_editor.h"
#include <application.h>
#include <imgui-filebrowser/imfilebrowser.h>

namespace ed = ax::NodeEditor;

struct LowCodeForITKApplication : public Application
{
    using Application::Application;

    void OnStart() override;

    void OnStop() override;

    void buttonForTriggeringEvaluation();
    void ButtonForSaving();
    void ButtonForLoading();

    void serialize();
    void deserialize();

    void OnFrame(float deltaTime) override;

    void drawMenu();

    void ImGuiDemoModal();

    void buttonForNodesModal();

    void drawingLinks();

    void pinsVisualLinking();

    void showFPS();
    void handleDeleting();

    void linksDeletion();

    ax::NodeEditor::EditorContext *m_Context    = nullptr; // Editor context, required to trace a editor state.
    bool                           m_FirstFrame = true;    // Flag set for first frame only, some action need
                                                           // to be executed once.

    Logic                    m_logic;
    ed::Config               m_config;
    inline static const auto settingsFile = "LowCodeForITKApplication.json";
};

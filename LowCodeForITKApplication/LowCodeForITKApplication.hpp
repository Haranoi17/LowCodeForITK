#include "DrawVisitor/Interface/DrawVisitor.hpp"
#include "Logic/Logic.hpp"
#include "Nodes/Interface/Node.hpp"
#include "Pins/Interface/Pin.hpp"
#include "UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"
#include "imgui_node_editor.h"
#include <application.h>
#include <imgui-filebrowser\imfilebrowser.h>

namespace ed = ax::NodeEditor;

struct LowCodeForITKApplication : public Application
{
    using Application::Application;

    void OnStart() override;

    void OnStop() override;

    void buttonForTriggeringEvaluation();

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
    int m_NextLinkId = 100; // Counter to help generate link ids. In real application this will
                            // probably based on pointer to user data structure.

    Logic                        m_logic;
    std::unique_ptr<DrawVisitor> m_drawVisitor;
};

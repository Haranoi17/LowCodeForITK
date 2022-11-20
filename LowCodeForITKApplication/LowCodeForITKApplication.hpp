#include <application.h>
#include "imgui_node_editor.h"
#include "Logic/Logic.hpp"
#include "DrawStrategy/Interface/DrawStrategy.hpp"
#include "UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"
#include "Logic/Interface/Node.hpp"
#include "Logic/Interface/Pin.hpp"

namespace ed = ax::NodeEditor;

struct LowCodeForITKApplication : public Application
{
	using Application::Application;

	void OnStart() override;

	void OnStop() override;

	void ImGuiEx_BeginColumn()
	{
		ImGui::BeginGroup();
	}

	void ImGuiEx_NextColumn()
	{
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
	}

	void ImGuiEx_EndColumn()
	{
		ImGui::EndGroup();
	}

	void buttonForTriggeringEvaluation();

	void OnFrame(float deltaTime) override;

	void drawingLinks();

	void pinsVisualLinking();

	void showFPS();
	void handleDeleting();

	ax::NodeEditor::EditorContext* m_Context = nullptr;    // Editor context, required to trace a editor state.
	bool                 m_FirstFrame = true;    // Flag set for first frame only, some action need to be executed once.
	int                  m_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.

	Logic m_logic;
	std::shared_ptr<DrawStrategy> m_drawStrategy;
};


#pragma once
#include <imgui_node_editor.h>
#include <application.h>
#include "CustomNodes/Interface/CustomNode.hpp"
#include "Identification/UniqueIDProvider/Interface/UniqueIDProvider.hpp"

#include <vector>
#include <memory>

namespace ed = ax::NodeEditor;

struct LowCodeForITKGui final : public Application
{
	using Application::Application;

	void OnStart() override;

	void OnStop() override;

	void OnFrame(float deltaTime) override;

	ed::EditorContext* m_Context = nullptr;

private:

	std::shared_ptr<UniqueIDProvider> m_idProvider;
	std::vector<std::unique_ptr<CustomNode>> m_nodes;
};

int Main(int argc, char** argv);
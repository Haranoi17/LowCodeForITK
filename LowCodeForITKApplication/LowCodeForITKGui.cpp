#include "LowCodeForITKGui.hpp"
#include "CustomNodes/SampleNode/SampleNode.hpp"
#include "UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"

#include <algorithm>

void LowCodeForITKGui::OnStart()
{
	ed::Config config;
	config.SettingsFile = "Simple.json";
	m_Context = ed::CreateEditor(&config);

	m_idProvider = std::make_shared<SimpleIDProvider>();
	m_nodes.push_back(std::make_unique<SampleNode>(m_idProvider));
	m_nodes.push_back(std::make_unique<SampleNode>(m_idProvider));
}

void LowCodeForITKGui::OnStop()
{
	ed::DestroyEditor(m_Context);
}

void LowCodeForITKGui::OnFrame(float deltaTime)
{
	auto& io = ImGui::GetIO();

	ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

	ImGui::Separator();

	m_idProvider->freeID(0);
	ed::SetCurrentEditor(m_Context);
	ed::Begin("My Editor", ImVec2(0.0, 0.0f));
	std::for_each(std::begin(m_nodes), std::end(m_nodes), [](std::unique_ptr<CustomNode>& node) {node->draw();});
	ed::End();
	ed::SetCurrentEditor(nullptr);

	//ImGui::ShowMetricsWindow();
}
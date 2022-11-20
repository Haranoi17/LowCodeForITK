#include "LowCodeForITKApplication.hpp"
#include <algorithm>
#include "DrawStrategy/ImguiNodeEditorDrawStrategy.hpp"

void LowCodeForITKApplication::OnStart()
{
	ed::Config config;
	config.SettingsFile = "LowCodeForITKApplication.json";
	m_Context = ed::CreateEditor(&config);

	m_drawStrategy = std::make_shared<ImguiNodeEditorDrawStrategy>();
	m_logic.init();
}

void LowCodeForITKApplication::OnStop()
{
	ed::DestroyEditor(m_Context);
}

void LowCodeForITKApplication::OnFrame(float deltaTime)
{
	showFPS();

	ImGui::Separator();

	ed::SetCurrentEditor(m_Context);

	ed::Begin("Editor", ImVec2(0.0, 0.0f));

	std::ranges::for_each(m_logic.getNodes(), [&](const std::unique_ptr<Node>& node) {m_drawStrategy->drawNode(node);});

	drawingLinks();

	pinsVisualLinking();

	handleDeleting();

	ed::End();//Editor

	if (m_FirstFrame)
		ed::NavigateToContent(0.0f);

	ed::SetCurrentEditor(nullptr);

	m_FirstFrame = false;
}

void LowCodeForITKApplication::handleDeleting()
{
	if (ed::BeginDelete())
	{
		ed::LinkId deletedLinkId;
		while (ed::QueryDeletedLink(&deletedLinkId))
		{
			if (ed::AcceptDeletedItem())
			{
				// Then remove link from your data.
				/*	for (auto& link : m_Links)
				{
				if (link.Id == deletedLinkId)
				{
				m_Links.erase(&link);
				break;
				}
				}*/
			}

			// You may reject link deletion by calling:
			// ed::RejectDeletedItem();
		}
	}
	ed::EndDelete();

}

void LowCodeForITKApplication::showFPS()
{
	auto& io = ImGui::GetIO();

	ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
}

void LowCodeForITKApplication::drawingLinks()
{
	for (auto& linkInfo : m_logic.getLinks())
		ed::Link(linkInfo->id, linkInfo->pinIds.first, linkInfo->pinIds.second);
}

void LowCodeForITKApplication::pinsVisualLinking()
{
	if (ed::BeginCreate())
	{
		ed::PinId inputPinId, outputPinId;
		if (ed::QueryNewLink(&inputPinId, &outputPinId))
		{
			if (m_logic.isLinkPossible(std::make_pair(inputPinId.Get(), outputPinId.Get())))
			{
				if (ed::AcceptNewItem())
				{
					m_logic.createLink(std::make_pair(inputPinId.Get(), outputPinId.Get()));
					//ed::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
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

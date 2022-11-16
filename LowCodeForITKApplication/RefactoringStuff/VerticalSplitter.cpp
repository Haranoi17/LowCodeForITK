#include "VerticalSplitter.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

void VerticalSplitter::split()
{
	using namespace ImGui;
	constexpr float splitter_long_axis_size{ -1.0f };
	constexpr float thickness{ 4.0f };

	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;

	auto firstWidth = m_resizeables.first->getWidth();
	auto secondWidth = m_resizeables.second->getWidth();
	bb.Min = window->DC.CursorPos + ImVec2(firstWidth, 0);
	bb.Max = bb.Min + CalcItemSize(ImVec2(thickness, splitter_long_axis_size), 0.0f, 0.0f);
	SplitterBehavior(bb, id, ImGuiAxis_X, &firstWidth, &secondWidth, 50, 50, 0.0f);

	const auto newFirstWidth = firstWidth;
	const auto newSecondWidth = secondWidth;
	m_resizeables.first->resize(ImVec2{ newFirstWidth, m_resizeables.first->getHeight() });
	m_resizeables.second->resize(ImVec2{ newSecondWidth, m_resizeables.second->getHeight() });
}

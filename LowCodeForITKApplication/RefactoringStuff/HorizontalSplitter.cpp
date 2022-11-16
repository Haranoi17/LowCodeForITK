#include "HorizontalSplitter.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

void HorizontalSplitter::split()
{
	using namespace ImGui;
	constexpr float splitter_long_axis_size{ -1.0f };
	constexpr float thickness{ 4.0f };

	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiID id = window->GetID("##Splitter");
	ImRect bb;

	auto firstHeight = m_resizeables.first->getHeight();
	auto secondHeight = m_resizeables.second->getHeight();
	bb.Min = window->DC.CursorPos + ImVec2(0, firstHeight);
	bb.Max = bb.Min + CalcItemSize(ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
	SplitterBehavior(bb, id, ImGuiAxis_Y, &firstHeight, &secondHeight, 50, 50, 0.0f);

	const auto newFirstHeigt = firstHeight;
	const auto newSecondHeigt = secondHeight;
	m_resizeables.first->resize(ImVec2{ m_resizeables.first->getWidth(), newFirstHeigt });
	m_resizeables.second->resize(ImVec2{ m_resizeables.second->getWidth(), newSecondHeigt });
}

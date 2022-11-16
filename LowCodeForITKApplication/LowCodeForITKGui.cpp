#include "LowCodeForITKGui.hpp"

ImRect ImGui_GetItemRect()
{
	return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
{
	auto result = rect;
	result.Min.x -= x;
	result.Min.y -= y;
	result.Max.x += x;
	result.Max.y += y;
	return result;
}

Link::Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId) :
	ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
{
}

Node* FindNode(ed::NodeId id)
{
	for (auto& node : m_Nodes)
		if (node->ID == id)
			return node.get();

	return nullptr;
}

Link* FindLink(ed::LinkId id)
{
	for (auto& link : m_Links)
		if (link.ID == id)
			return &link;

	return nullptr;
}

Pin* FindPin(ed::PinId id)
{
	if (!id)
		return nullptr;

	for (auto& node : m_Nodes)
	{
		for (auto& pin : node->Inputs)
			if (pin.ID == id)
				return &pin;

		for (auto& pin : node->Outputs)
			if (pin.ID == id)
				return &pin;
	}

	return nullptr;
}

bool IsPinLinked(ed::PinId id)
{
	if (!id)
		return false;

	for (auto& link : m_Links)
		if (link.StartPinID == id || link.EndPinID == id)
			return true;

	return false;
}

bool CanCreateLink(Pin* a, Pin* b)
{
	if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
		return false;

	return true;
}

void BlueprintsBaseApp::OnStart()
{
	ed::Config config;

	config.SettingsFile = "Blueprints.json";

	config.UserPointer = this;

	config.LoadNodeSettings = [](ed::NodeId nodeId, char* data, void* userPointer) -> size_t
	{
		auto self = static_cast<BlueprintsBaseApp*>(userPointer);

		auto node = FindNode(nodeId);
		if (!node)
			return 0;

		if (data != nullptr)
			memcpy(data, node->State.data(), node->State.size());
		return node->State.size();
	};

	config.SaveNodeSettings = [](ed::NodeId nodeId, const char* data, size_t size, ed::SaveReasonFlags reason, void* userPointer) -> bool
	{
		auto self = static_cast<BlueprintsBaseApp*>(userPointer);

		auto node = FindNode(nodeId);
		if (!node)
			return false;

		node->State.assign(data, size);


		return true;
	};

	m_Editor = ed::CreateEditor(&config);
	ed::SetCurrentEditor(m_Editor);


	m_Nodes.push_back(m_nodeCreator->SpawnInputActionNode());      ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(-252, 220));
	m_Nodes.push_back(m_nodeCreator->SpawnBranchNode());           ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(-300, 351));
	m_Nodes.push_back(m_nodeCreator->SpawnDoNNode());              ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(-238, 504));
	m_Nodes.push_back(m_nodeCreator->SpawnOutputActionNode());     ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(71, 80));
	m_Nodes.push_back(m_nodeCreator->SpawnSetTimerNode());         ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(168, 316));
	m_Nodes.push_back(m_nodeCreator->SpawnTreeSequenceNode());     ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(1028, 329));
	m_Nodes.push_back(m_nodeCreator->SpawnTreeTaskNode());         ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(1204, 458));
	m_Nodes.push_back(m_nodeCreator->SpawnTreeTask2Node());        ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(868, 538));
	m_Nodes.push_back(m_nodeCreator->SpawnComment());              ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(112, 576)); ed::SetGroupSize(m_Nodes.back()->ID, ImVec2(384, 154));
	m_Nodes.push_back(m_nodeCreator->SpawnComment());              ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(800, 224)); ed::SetGroupSize(m_Nodes.back()->ID, ImVec2(640, 400));
	m_Nodes.push_back(m_nodeCreator->SpawnLessNode());             ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(366, 652));
	m_Nodes.push_back(m_nodeCreator->SpawnWeirdNode());            ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(144, 652));
	m_Nodes.push_back(m_nodeCreator->SpawnMessageNode());          ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(-348, 698));
	m_Nodes.push_back(m_nodeCreator->SpawnPrintStringNode());      ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(-69, 652));
	m_Nodes.push_back(m_nodeCreator->SpawnHoudiniTransformNode()); ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(500, -70));
	m_Nodes.push_back(m_nodeCreator->SpawnHoudiniGroupNode());     ed::SetNodePosition(m_Nodes.back()->ID, ImVec2(500, 42));

	ed::NavigateToContent();

	m_Links.push_back(Link(m_idProvider->generateID(), m_Nodes[5]->Outputs[0].ID, m_Nodes[6]->Inputs[0].ID));
	m_Links.push_back(Link(m_idProvider->generateID(), m_Nodes[5]->Outputs[0].ID, m_Nodes[7]->Inputs[0].ID));

	m_Links.push_back(Link(m_idProvider->generateID(), m_Nodes[14]->Outputs[0].ID, m_Nodes[15]->Inputs[0].ID));

	m_HeaderBackground = LoadTexture("data/BlueprintBackground.png");
	m_SaveIcon = LoadTexture("data/ic_save_white_24dp.png");
}

void BlueprintsBaseApp::OnStop()
{
	auto releaseTexture = [this](ImTextureID& id)
	{
		if (id)
		{
			DestroyTexture(id);
			id = nullptr;
		}
	};

	releaseTexture(m_RestoreIcon);
	releaseTexture(m_SaveIcon);
	releaseTexture(m_HeaderBackground);

	if (m_Editor)
	{
		ed::DestroyEditor(m_Editor);
		m_Editor = nullptr;
	}
}

ImColor GetIconColor(PinType type)
{
	switch (type)
	{
	default:
	case PinType::Flow:     return ImColor(255, 255, 255);
	case PinType::Bool:     return ImColor(220, 48, 48);
	case PinType::Int:      return ImColor(68, 201, 156);
	case PinType::Float:    return ImColor(147, 226, 74);
	case PinType::String:   return ImColor(124, 21, 153);
	case PinType::Object:   return ImColor(51, 150, 215);
	case PinType::Function: return ImColor(218, 0, 183);
	case PinType::Delegate: return ImColor(255, 48, 48);
	}
}

void DrawPinIcon(const Pin& pin, bool connected, int alpha)
{
	IconType iconType;
	ImColor  color = GetIconColor(pin.Type);
	color.Value.w = alpha / 255.0f;
	switch (pin.Type)
	{
	case PinType::Flow:     iconType = IconType::Flow;   break;
	case PinType::Bool:     iconType = IconType::Circle; break;
	case PinType::Int:      iconType = IconType::Circle; break;
	case PinType::Float:    iconType = IconType::Circle; break;
	case PinType::String:   iconType = IconType::Circle; break;
	case PinType::Object:   iconType = IconType::Circle; break;
	case PinType::Function: iconType = IconType::Circle; break;
	case PinType::Delegate: iconType = IconType::Square; break;
	default:
		return;
	}

	ax::Widgets::Icon(ImVec2(static_cast<float>(m_PinIconSize), static_cast<float>(m_PinIconSize)), iconType, connected, color, ImColor(32, 32, 32, alpha));
}

void BlueprintsBaseApp::OnFrame(float deltaTime)
{
	showFPS();
	ed::SetCurrentEditor(m_Editor);

	m_verticalSplitter->split();
	m_leftPane->draw();
	ImGui::SameLine(0.0f, 12.0f);
	m_nodeEditorPane->draw();
}

void handleOrdinals(const ImVec2& editorMin, const ImVec2& editorMax)
{
	if (m_ShowOrdinals)
	{
		int nodeCount = ed::GetNodeCount();
		std::vector<ed::NodeId> orderedNodeIds;
		orderedNodeIds.resize(static_cast<size_t>(nodeCount));
		ed::GetOrderedNodeIds(orderedNodeIds.data(), nodeCount);


		auto drawList = ImGui::GetWindowDrawList();
		drawList->PushClipRect(editorMin, editorMax);

		int ordinal = 0;
		for (auto& nodeId : orderedNodeIds)
		{
			auto p0 = ed::GetNodePosition(nodeId);
			auto p1 = p0 + ed::GetNodeSize(nodeId);
			p0 = ed::CanvasToScreen(p0);
			p1 = ed::CanvasToScreen(p1);


			ImGuiTextBuffer builder;
			builder.appendf("#%d", ordinal++);

			auto textSize = ImGui::CalcTextSize(builder.c_str());
			auto padding = ImVec2(2.0f, 2.0f);
			auto widgetSize = textSize + padding * 2;

			auto widgetPosition = ImVec2(p1.x, p0.y) + ImVec2(0.0f, -widgetSize.y);

			drawList->AddRectFilled(widgetPosition, widgetPosition + widgetSize, IM_COL32(100, 80, 80, 190), 3.0f, ImDrawFlags_RoundCornersAll);
			drawList->AddRect(widgetPosition, widgetPosition + widgetSize, IM_COL32(200, 160, 160, 190), 3.0f, ImDrawFlags_RoundCornersAll);
			drawList->AddText(widgetPosition + padding, IM_COL32(255, 255, 255, 255), builder.c_str());
		}

		drawList->PopClipRect();
	}
}

void pinDragLink(Pin*& newLinkPin, bool& createNewNode, Pin*& newNodeLinkPin)
{
	if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
	{
		auto showLabel = [](const char* label, ImColor color)
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
			auto size = ImGui::CalcTextSize(label);

			auto padding = ImGui::GetStyle().FramePadding;
			auto spacing = ImGui::GetStyle().ItemSpacing;

			ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

			auto rectMin = ImGui::GetCursorScreenPos() - padding;
			auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

			auto drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
			ImGui::TextUnformatted(label);
		};

		ed::PinId startPinId = 0, endPinId = 0;
		if (ed::QueryNewLink(&startPinId, &endPinId))
		{
			auto startPin = FindPin(startPinId);
			auto endPin = FindPin(endPinId);

			newLinkPin = startPin ? startPin : endPin;

			if (startPin->Kind == PinKind::Input)
			{
				std::swap(startPin, endPin);
				std::swap(startPinId, endPinId);
			}

			if (startPin && endPin)
			{
				if (endPin == startPin)
				{
					ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (endPin->Kind == startPin->Kind)
				{
					showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
					ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (endPin->Type != startPin->Type)
				{
					showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
					ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
				}
				else
				{
					showLabel("+ Create Link", ImColor(32, 45, 32, 180));
					if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
					{
						m_Links.emplace_back(Link(m_idProvider->generateID(), startPinId, endPinId));
						m_Links.back().Color = GetIconColor(startPin->Type);
					}
				}
			}
		}

		ed::PinId pinId = 0;
		if (ed::QueryNewNode(&pinId))
		{
			newLinkPin = FindPin(pinId);
			if (newLinkPin)
				showLabel("+ Create Node", ImColor(32, 45, 32, 180));

			if (ed::AcceptNewItem())
			{
				createNewNode = true;
				newNodeLinkPin = FindPin(pinId);
				newLinkPin = nullptr;
				ed::Suspend();
				ImGui::OpenPopup("Create New Node");
				ed::Resume();
			}
		}
	}
	else
		newLinkPin = nullptr;

	ed::EndCreate();
}

void handleGeneralDelete()
{
	if (ed::BeginDelete())
	{
		ed::LinkId linkId = 0;
		while (ed::QueryDeletedLink(&linkId))
		{
			if (ed::AcceptDeletedItem())
			{
				auto id = std::find_if(m_Links.begin(), m_Links.end(), [linkId](auto& link) { return link.ID == linkId; });
				if (id != m_Links.end())
					m_Links.erase(id);
			}
		}

		ed::NodeId nodeId = 0;
		while (ed::QueryDeletedNode(&nodeId))
		{
			if (ed::AcceptDeletedItem())
			{
				auto id = std::find_if(m_Nodes.begin(), m_Nodes.end(), [nodeId](auto& node) { return node->ID == nodeId; });
				if (id != m_Nodes.end())
					m_Nodes.erase(id);
			}
		}
	}
	ed::EndDelete();
}

void handleLinks()
{
	for (auto& link : m_Links)
		ed::Link(link.ID, link.StartPinID, link.EndPinID, link.Color, 2.0f);
}

void handleNodes4()
{
	for (auto& node : m_Nodes)
	{
		if (node->Type != NodeType::Comment)
			continue;

		const float commentAlpha = 0.75f;

		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha);
		ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
		ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
		ed::BeginNode(node->ID);
		ImGui::PushID(node->ID.AsPointer());
		ImGui::BeginVertical("content");
		ImGui::BeginHorizontal("horizontal");
		ImGui::Spring(1);
		ImGui::TextUnformatted(node->Name.c_str());
		ImGui::Spring(1);
		ImGui::EndHorizontal();
		ed::Group(node->Size);
		ImGui::EndVertical();
		ImGui::PopID();
		ed::EndNode();
		ed::PopStyleColor(2);
		ImGui::PopStyleVar();

		if (ed::BeginGroupHint(node->ID))
		{
			auto bgAlpha = static_cast<int>(ImGui::GetStyle().Alpha * 255);

			auto min = ed::GetGroupMin();

			ImGui::SetCursorScreenPos(min - ImVec2(-8, ImGui::GetTextLineHeightWithSpacing() + 4));
			ImGui::BeginGroup();
			ImGui::TextUnformatted(node->Name.c_str());
			ImGui::EndGroup();

			auto drawList = ed::GetHintBackgroundDrawList();

			auto hintBounds = ImGui_GetItemRect();
			auto hintFrameBounds = ImRect_Expanded(hintBounds, 8, 4);

			drawList->AddRectFilled(
				hintFrameBounds.GetTL(),
				hintFrameBounds.GetBR(),
				IM_COL32(255, 255, 255, 64 * bgAlpha / 255), 4.0f);

			drawList->AddRect(
				hintFrameBounds.GetTL(),
				hintFrameBounds.GetBR(),
				IM_COL32(255, 255, 255, 128 * bgAlpha / 255), 4.0f);

		}
		ed::EndGroupHint();
	}
}

void handleNodes3(Pin* newLinkPin)
{
	for (auto& node : m_Nodes)
	{
		if (node->Type != NodeType::Houdini)
			continue;

		const float rounding = 10.0f;
		const float padding = 12.0f;


		ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(229, 229, 229, 200));
		ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(125, 125, 125, 200));
		ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(229, 229, 229, 60));
		ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(125, 125, 125, 60));

		const auto pinBackground = ed::GetStyle().Colors[ed::StyleColor_NodeBg];

		ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
		ed::PushStyleVar(ed::StyleVar_NodeRounding, rounding);
		ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
		ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
		ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
		ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
		ed::PushStyleVar(ed::StyleVar_PinRadius, 6.0f);
		ed::BeginNode(node->ID);

		ImGui::BeginVertical(node->ID.AsPointer());
		if (!node->Inputs.empty())
		{
			ImGui::BeginHorizontal("inputs");
			ImGui::Spring(1, 0);

			ImRect inputsRect;
			int inputAlpha = 200;
			for (auto& pin : node->Inputs)
			{
				ImGui::Dummy(ImVec2(padding, padding));
				inputsRect = ImGui_GetItemRect();
				ImGui::Spring(1, 0);
				inputsRect.Min.y -= padding;
				inputsRect.Max.y -= padding;

				const auto allRoundCornersFlags = ImDrawFlags_RoundCornersAll;

				ed::PushStyleVar(ed::StyleVar_PinCorners, allRoundCornersFlags);

				ed::BeginPin(pin.ID, ed::PinKind::Input);
				ed::PinPivotRect(inputsRect.GetCenter(), inputsRect.GetCenter());
				ed::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
				ed::EndPin();
				ed::PopStyleVar(1);

				auto drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(inputsRect.GetTL(), inputsRect.GetBR(),
					IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, allRoundCornersFlags);
				drawList->AddRect(inputsRect.GetTL(), inputsRect.GetBR(),
					IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, allRoundCornersFlags);

				if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
					inputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
			}

			ImGui::EndHorizontal();
		}

		ImGui::BeginHorizontal("content_frame");
		ImGui::Spring(1, padding);

		ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
		ImGui::Dummy(ImVec2(160, 0));
		ImGui::Spring(1);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
		ImGui::TextUnformatted(node->Name.c_str());
		ImGui::PopStyleColor();
		ImGui::Spring(1);
		ImGui::EndVertical();
		auto contentRect = ImGui_GetItemRect();

		ImGui::Spring(1, padding);
		ImGui::EndHorizontal();

		if (!node->Outputs.empty())
		{
			ImGui::BeginHorizontal("outputs");
			ImGui::Spring(1, 0);

			ImRect outputsRect;
			int outputAlpha = 200;
			for (auto& pin : node->Outputs)
			{
				ImGui::Dummy(ImVec2(padding, padding));
				outputsRect = ImGui_GetItemRect();
				ImGui::Spring(1, 0);
				outputsRect.Min.y += padding;
				outputsRect.Max.y += padding;

				const auto allRoundCornersFlags = ImDrawFlags_RoundCornersAll;
				const auto topRoundCornersFlags = ImDrawFlags_RoundCornersTop;

				ed::PushStyleVar(ed::StyleVar_PinCorners, topRoundCornersFlags);
				ed::BeginPin(pin.ID, ed::PinKind::Output);
				ed::PinPivotRect(outputsRect.GetCenter(), outputsRect.GetCenter());
				ed::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
				ed::EndPin();
				ed::PopStyleVar();


				auto drawList = ImGui::GetWindowDrawList();
				drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR(),
					IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, allRoundCornersFlags);
				drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR(),
					IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, allRoundCornersFlags);


				if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
					outputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
			}

			ImGui::EndHorizontal();
		}

		ImGui::EndVertical();

		ed::EndNode();
		ed::PopStyleVar(7);
		ed::PopStyleColor(4);
	}
}

void handleNodes2(Pin* newLinkPin)
{
	for (auto& node : m_Nodes)
	{
		if (node->Type != NodeType::Tree)
			continue;

		const float rounding = 5.0f;
		const float padding = 12.0f;

		const auto pinBackground = ed::GetStyle().Colors[ed::StyleColor_NodeBg];

		ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
		ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(32, 32, 32, 200));
		ed::PushStyleColor(ed::StyleColor_PinRect, ImColor(60, 180, 255, 150));
		ed::PushStyleColor(ed::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

		ed::PushStyleVar(ed::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
		ed::PushStyleVar(ed::StyleVar_NodeRounding, rounding);
		ed::PushStyleVar(ed::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
		ed::PushStyleVar(ed::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
		ed::PushStyleVar(ed::StyleVar_LinkStrength, 0.0f);
		ed::PushStyleVar(ed::StyleVar_PinBorderWidth, 1.0f);
		ed::PushStyleVar(ed::StyleVar_PinRadius, 5.0f);
		ed::BeginNode(node->ID);

		ImGui::BeginVertical(node->ID.AsPointer());
		ImGui::BeginHorizontal("inputs");
		ImGui::Spring(0, padding * 2);

		ImRect inputsRect;
		int inputAlpha = 200;
		if (!node->Inputs.empty())
		{
			auto& pin = node->Inputs[0];
			ImGui::Dummy(ImVec2(0, padding));
			ImGui::Spring(1, 0);
			inputsRect = ImGui_GetItemRect();

			ed::PushStyleVar(ed::StyleVar_PinArrowSize, 10.0f);
			ed::PushStyleVar(ed::StyleVar_PinArrowWidth, 10.0f);

			ed::PushStyleVar(ed::StyleVar_PinCorners, ImDrawFlags_RoundCornersBottom);

			ed::BeginPin(pin.ID, ed::PinKind::Input);
			ed::PinPivotRect(inputsRect.GetTL(), inputsRect.GetBR());
			ed::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
			ed::EndPin();
			ed::PopStyleVar(3);

			if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
				inputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
		}
		else
			ImGui::Dummy(ImVec2(0, padding));

		ImGui::Spring(0, padding * 2);
		ImGui::EndHorizontal();

		ImGui::BeginHorizontal("content_frame");
		ImGui::Spring(1, padding);

		ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
		ImGui::Dummy(ImVec2(160, 0));
		ImGui::Spring(1);
		ImGui::TextUnformatted(node->Name.c_str());
		ImGui::Spring(1);
		ImGui::EndVertical();
		auto contentRect = ImGui_GetItemRect();

		ImGui::Spring(1, padding);
		ImGui::EndHorizontal();

		ImGui::BeginHorizontal("outputs");
		ImGui::Spring(0, padding * 2);

		ImRect outputsRect;
		int outputAlpha = 200;
		if (!node->Outputs.empty())
		{
			auto& pin = node->Outputs[0];
			ImGui::Dummy(ImVec2(0, padding));
			ImGui::Spring(1, 0);
			outputsRect = ImGui_GetItemRect();

			ed::PushStyleVar(ed::StyleVar_PinCorners, ImDrawFlags_RoundCornersTop);

			ed::BeginPin(pin.ID, ed::PinKind::Output);
			ed::PinPivotRect(outputsRect.GetTL(), outputsRect.GetBR());
			ed::PinRect(outputsRect.GetTL(), outputsRect.GetBR());
			ed::EndPin();
			ed::PopStyleVar();

			if (newLinkPin && !CanCreateLink(newLinkPin, &pin) && &pin != newLinkPin)
				outputAlpha = (int)(255 * ImGui::GetStyle().Alpha * (48.0f / 255.0f));
		}
		else
			ImGui::Dummy(ImVec2(0, padding));

		ImGui::Spring(0, padding * 2);
		ImGui::EndHorizontal();

		ImGui::EndVertical();

		ed::EndNode();
		ed::PopStyleVar(7);
		ed::PopStyleColor(4);

		auto drawList = ed::GetNodeBackgroundDrawList(node->ID);

		const auto    topRoundCornersFlags = ImDrawFlags_RoundCornersTop;
		const auto bottomRoundCornersFlags = ImDrawFlags_RoundCornersBottom;

		drawList->AddRectFilled(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
			IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
		drawList->AddRect(inputsRect.GetTL() + ImVec2(0, 1), inputsRect.GetBR(),
			IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, bottomRoundCornersFlags);
		drawList->AddRectFilled(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
			IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
		drawList->AddRect(outputsRect.GetTL(), outputsRect.GetBR() - ImVec2(0, 1),
			IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, topRoundCornersFlags);
		drawList->AddRectFilled(contentRect.GetTL(), contentRect.GetBR(), IM_COL32(24, 64, 128, 200), 0.0f);
		drawList->AddRect(
			contentRect.GetTL(),
			contentRect.GetBR(),
			IM_COL32(48, 128, 255, 100), 0.0f);
	}
}

void handleNodes(ax::NodeEditor::Utilities::BlueprintNodeBuilder& builder, Pin* newLinkPin)
{
	for (auto& node : m_Nodes)
	{
		if (node->Type != NodeType::Blueprint && node->Type != NodeType::Simple)
			continue;

		const auto isSimple = node->Type == NodeType::Simple;

		bool hasOutputDelegates = false;
		for (auto& output : node->Outputs)
			if (output.Type == PinType::Delegate)
				hasOutputDelegates = true;

		builder.Begin(node->ID);
		if (!isSimple)
		{
			builder.Header(node->Color);
			ImGui::Spring(0);
			ImGui::TextUnformatted(node->Name.c_str());
			ImGui::Spring(1);
			ImGui::Dummy(ImVec2(0, 28));
			if (hasOutputDelegates)
			{
				ImGui::BeginVertical("delegates", ImVec2(0, 28));
				ImGui::Spring(1, 0);
				for (auto& output : node->Outputs)
				{
					if (output.Type != PinType::Delegate)
						continue;

					auto alpha = ImGui::GetStyle().Alpha;
					if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
						alpha = alpha * (48.0f / 255.0f);

					ed::BeginPin(output.ID, ed::PinKind::Output);
					ed::PinPivotAlignment(ImVec2(1.0f, 0.5f));
					ed::PinPivotSize(ImVec2(0, 0));
					ImGui::BeginHorizontal(output.ID.AsPointer());
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
					if (!output.Name.empty())
					{
						ImGui::TextUnformatted(output.Name.c_str());
						ImGui::Spring(0);
					}
					DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
					ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
					ImGui::EndHorizontal();
					ImGui::PopStyleVar();
					ed::EndPin();

					//DrawItemRect(ImColor(255, 0, 0));
				}
				ImGui::Spring(1, 0);
				ImGui::EndVertical();
				ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
			}
			else
				ImGui::Spring(0);
			builder.EndHeader();
		}

		for (auto& input : node->Inputs)
		{
			auto alpha = ImGui::GetStyle().Alpha;
			if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
				alpha = alpha * (48.0f / 255.0f);

			builder.Input(input.ID);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
			DrawPinIcon(input, IsPinLinked(input.ID), (int)(alpha * 255));
			ImGui::Spring(0);
			if (!input.Name.empty())
			{
				ImGui::TextUnformatted(input.Name.c_str());
				ImGui::Spring(0);
			}
			if (input.Type == PinType::Bool)
			{
				ImGui::Button("Hello");
				ImGui::Spring(0);
			}
			ImGui::PopStyleVar();
			builder.EndInput();
		}

		if (isSimple)
		{
			builder.Middle();

			ImGui::Spring(1, 0);
			ImGui::TextUnformatted(node->Name.c_str());
			ImGui::Spring(1, 0);
		}

		for (auto& output : node->Outputs)
		{
			if (!isSimple && output.Type == PinType::Delegate)
				continue;

			auto alpha = ImGui::GetStyle().Alpha;
			if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
				alpha = alpha * (48.0f / 255.0f);

			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
			builder.Output(output.ID);
			if (output.Type == PinType::String)
			{
				static char buffer[128] = "Edit Me\nMultiline!";
				static bool wasActive = false;

				ImGui::PushItemWidth(100.0f);
				ImGui::InputText("##edit", buffer, 127);
				ImGui::PopItemWidth();
				if (ImGui::IsItemActive() && !wasActive)
				{
					ed::EnableShortcuts(false);
					wasActive = true;
				}
				else if (!ImGui::IsItemActive() && wasActive)
				{
					ed::EnableShortcuts(true);
					wasActive = false;
				}
				ImGui::Spring(0);
			}
			if (!output.Name.empty())
			{
				ImGui::Spring(0);
				ImGui::TextUnformatted(output.Name.c_str());
			}
			ImGui::Spring(0);
			DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
			ImGui::PopStyleVar();
			builder.EndOutput();
		}

		builder.End();
	}
}

void popupsBullshit(Pin*& newNodeLinkPin, bool& createNewNode)
{
	auto openPopupPosition = ImGui::GetMousePos();
	ed::Suspend();
	if (ed::ShowNodeContextMenu(&m_contextNodeId))
		ImGui::OpenPopup("Node Context Menu");
	else if (ed::ShowPinContextMenu(&m_contextPinId))
		ImGui::OpenPopup("Pin Context Menu");
	else if (ed::ShowLinkContextMenu(&m_contextLinkId))
		ImGui::OpenPopup("Link Context Menu");
	else if (ed::ShowBackgroundContextMenu())
	{
		ImGui::OpenPopup("Create New Node");
		newNodeLinkPin = nullptr;
	}
	ed::Resume();

	ed::Suspend();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("Node Context Menu"))
	{
		auto node = FindNode(m_contextNodeId);

		ImGui::TextUnformatted("Node Context Menu");
		ImGui::Separator();
		if (node)
		{
			ImGui::Text("ID: %p", node->ID.AsPointer());
			ImGui::Text("Type: %s", node->Type == NodeType::Blueprint ? "Blueprint" : (node->Type == NodeType::Tree ? "Tree" : "Comment"));
			ImGui::Text("Inputs: %d", (int)node->Inputs.size());
			ImGui::Text("Outputs: %d", (int)node->Outputs.size());
		}
		else
			ImGui::Text("Unknown node-> %p", m_contextNodeId.AsPointer());
		ImGui::Separator();
		if (ImGui::MenuItem("Delete"))
			ed::DeleteNode(m_contextNodeId);
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Pin Context Menu"))
	{
		auto pin = FindPin(m_contextPinId);

		ImGui::TextUnformatted("Pin Context Menu");
		ImGui::Separator();
		if (pin)
		{
			ImGui::Text("ID: %p", pin->ID.AsPointer());
			if (pin->Node)
				ImGui::Text("Node: %p", pin->Node->ID.AsPointer());
			else
				ImGui::Text("Node: %s", "<none>");
		}
		else
			ImGui::Text("Unknown pin: %p", m_contextPinId.AsPointer());

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Link Context Menu"))
	{
		auto link = FindLink(m_contextLinkId);

		ImGui::TextUnformatted("Link Context Menu");
		ImGui::Separator();
		if (link)
		{
			ImGui::Text("ID: %p", link->ID.AsPointer());
			ImGui::Text("From: %p", link->StartPinID.AsPointer());
			ImGui::Text("To: %p", link->EndPinID.AsPointer());
		}
		else
			ImGui::Text("Unknown link: %p", m_contextLinkId.AsPointer());
		ImGui::Separator();
		if (ImGui::MenuItem("Delete"))
			ed::DeleteLink(m_contextLinkId);
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Create New Node"))
	{
		auto newNodePostion = openPopupPosition;

		std::unique_ptr<Node> node = nullptr;
		if (ImGui::MenuItem("Input Action"))
			node = m_nodeCreator->SpawnInputActionNode();
		if (ImGui::MenuItem("Output Action"))
			node = m_nodeCreator->SpawnOutputActionNode();
		if (ImGui::MenuItem("Branch"))
			node = m_nodeCreator->SpawnBranchNode();
		if (ImGui::MenuItem("Do N"))
			node = m_nodeCreator->SpawnDoNNode();
		if (ImGui::MenuItem("Set Timer"))
			node = m_nodeCreator->SpawnSetTimerNode();
		if (ImGui::MenuItem("Less"))
			node = m_nodeCreator->SpawnLessNode();
		if (ImGui::MenuItem("Weird"))
			node = m_nodeCreator->SpawnWeirdNode();
		if (ImGui::MenuItem("Trace by Channel"))
			node = m_nodeCreator->SpawnTraceByChannelNode();
		if (ImGui::MenuItem("Print String"))
			node = m_nodeCreator->SpawnPrintStringNode();
		ImGui::Separator();
		if (ImGui::MenuItem("Comment"))
			node = m_nodeCreator->SpawnComment();
		ImGui::Separator();
		if (ImGui::MenuItem("Sequence"))
			node = m_nodeCreator->SpawnTreeSequenceNode();
		if (ImGui::MenuItem("Move To"))
			node = m_nodeCreator->SpawnTreeTaskNode();
		if (ImGui::MenuItem("Random Wait"))
			node = m_nodeCreator->SpawnTreeTask2Node();
		ImGui::Separator();
		if (ImGui::MenuItem("Message"))
			node = m_nodeCreator->SpawnMessageNode();
		ImGui::Separator();
		if (ImGui::MenuItem("Transform"))
			node = m_nodeCreator->SpawnHoudiniTransformNode();
		if (ImGui::MenuItem("Group"))
			node = m_nodeCreator->SpawnHoudiniGroupNode();

		if (node)
		{
			createNewNode = false;

			ed::SetNodePosition(node->ID, newNodePostion);

			if (auto startPin = newNodeLinkPin)
			{
				auto& pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;

				for (auto& pin : pins)
				{
					if (CanCreateLink(startPin, &pin))
					{
						auto endPin = &pin;
						if (startPin->Kind == PinKind::Input)
							std::swap(startPin, endPin);

						m_Links.emplace_back(Link(m_idProvider->generateID(), startPin->ID, endPin->ID));
						m_Links.back().Color = GetIconColor(startPin->Type);

						break;
					}
				}
			}

			m_Nodes.push_back(std::move(node));
		}


		ImGui::EndPopup();
	}
	else
		createNewNode = false;
	ImGui::PopStyleVar();
	ed::Resume();

	ed::End();
}

void BlueprintsBaseApp::showFPS()
{
	auto& io = ImGui::GetIO();

	ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
}

void NodeEditorPane::draw()
{
	ed::Begin("Node editor", m_size);
	{
		auto cursorTopLeft = ImGui::GetCursorScreenPos();

		//util::BlueprintNodeBuilder builder(m_HeaderBackground, GetTextureWidth(m_HeaderBackground), GetTextureHeight(m_HeaderBackground));
		util::BlueprintNodeBuilder builder(m_HeaderBackground, 100, 100);
		handleNodes(builder, newLinkPin);

		handleNodes2(newLinkPin);

		handleNodes3(newLinkPin);

		handleNodes4();

		handleLinks();

		if (!createNewNode)
		{
			pinDragLink(newLinkPin, createNewNode, newNodeLinkPin);

			handleGeneralDelete();
		}

		ImGui::SetCursorScreenPos(cursorTopLeft);
	}

	popupsBullshit(newNodeLinkPin, createNewNode);

	auto editorMin = ImGui::GetItemRectMin();
	auto editorMax = ImGui::GetItemRectMax();

	handleOrdinals(editorMin, editorMax);
}

void LeftPane::draw()
{
	auto& io = ImGui::GetIO();
	ImGui::BeginChild("Selection", ImVec2(m_size.x, 0));

	m_size.x = ImGui::GetContentRegionAvail().x;

	ImGui::BeginHorizontal("Style Editor", ImVec2(m_size.x, 0));
	ImGui::Spring(0.0f, 0.0f);
	if (ImGui::Button("Zoom to Content"))
		ed::NavigateToContent();
	ImGui::Spring(0.0f);
	if (ImGui::Button("Show Flow"))
	{
		for (auto& link : m_Links)
			ed::Flow(link.ID);
	}
	ImGui::Spring();

	m_styleWindow.drawIfNecessary();

	ImGui::EndHorizontal();
	ImGui::Checkbox("Show Ordinals", &m_ShowOrdinals);


	std::vector<ed::NodeId> selectedNodes;
	std::vector<ed::LinkId> selectedLinks;
	selectedNodes.resize(ed::GetSelectedObjectCount());
	selectedLinks.resize(ed::GetSelectedObjectCount());

	int nodeCount = ed::GetSelectedNodes(selectedNodes.data(), static_cast<int>(selectedNodes.size()));
	int linkCount = ed::GetSelectedLinks(selectedLinks.data(), static_cast<int>(selectedLinks.size()));

	selectedNodes.resize(nodeCount);
	selectedLinks.resize(linkCount);

	int saveIconWidth = 20;//GetTextureWidth(m_SaveIcon);
	int saveIconHeight = 20; //GetTextureWidth(m_SaveIcon);
	int restoreIconWidth = 20; //GetTextureWidth(m_RestoreIcon);
	int restoreIconHeight = 20; //GetTextureWidth(m_RestoreIcon);


	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetCursorScreenPos(),
		ImGui::GetCursorScreenPos() + ImVec2(m_size.x, ImGui::GetTextLineHeight()),
		ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::TextUnformatted("Nodes");
	ImGui::Indent();
	for (auto& node : m_Nodes)
	{
		ImGui::PushID(node->ID.AsPointer());
		auto start = ImGui::GetCursorScreenPos();

		bool isSelected = std::find(selectedNodes.begin(), selectedNodes.end(), node->ID) != selectedNodes.end();
		if (ImGui::Selectable((node->Name + "##" + std::to_string(reinterpret_cast<uintptr_t>(node->ID.AsPointer()))).c_str(), &isSelected))
		{
			if (io.KeyCtrl)
			{
				if (isSelected)
					ed::SelectNode(node->ID, true);
				else
					ed::DeselectNode(node->ID);
			}
			else
				ed::SelectNode(node->ID, false);

			ed::NavigateToSelection();
		}
		if (ImGui::IsItemHovered() && !node->State.empty())
			ImGui::SetTooltip("State: %s", node->State.c_str());

		auto id = std::string("(") + std::to_string(reinterpret_cast<uintptr_t>(node->ID.AsPointer())) + ")";
		auto textSize = ImGui::CalcTextSize(id.c_str(), nullptr);
		auto iconPanelPos = start + ImVec2(
			m_size.x - ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().IndentSpacing - saveIconWidth - restoreIconWidth - ImGui::GetStyle().ItemInnerSpacing.x * 1,
			(ImGui::GetTextLineHeight() - saveIconHeight) / 2);
		ImGui::GetWindowDrawList()->AddText(
			ImVec2(iconPanelPos.x - textSize.x - ImGui::GetStyle().ItemInnerSpacing.x, start.y),
			IM_COL32(255, 255, 255, 255), id.c_str(), nullptr);

		auto drawList = ImGui::GetWindowDrawList();
		ImGui::SetCursorScreenPos(iconPanelPos);
		ImGui::SetItemAllowOverlap();
		if (node->SavedState.empty())
		{
			if (ImGui::InvisibleButton("save", ImVec2((float)saveIconWidth, (float)saveIconHeight)))
				node->SavedState = node->State;

			if (ImGui::IsItemActive())
				drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
			else if (ImGui::IsItemHovered())
				drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
			else
				drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
		}
		else
		{
			ImGui::Dummy(ImVec2((float)saveIconWidth, (float)saveIconHeight));
			drawList->AddImage(m_SaveIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
		}

		ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
		ImGui::SetItemAllowOverlap();
		if (!node->SavedState.empty())
		{
			if (ImGui::InvisibleButton("restore", ImVec2((float)restoreIconWidth, (float)restoreIconHeight)))
			{
				node->State = node->SavedState;
				ed::RestoreNodeState(node->ID);
				node->SavedState.clear();
			}

			if (ImGui::IsItemActive())
				drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 96));
			else if (ImGui::IsItemHovered())
				drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
			else
				drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 160));
		}
		else
		{
			ImGui::Dummy(ImVec2((float)restoreIconWidth, (float)restoreIconHeight));
			drawList->AddImage(m_RestoreIcon, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 32));
		}

		ImGui::SameLine(0, 0);
		ImGui::SetItemAllowOverlap();
		ImGui::Dummy(ImVec2(0, (float)restoreIconHeight));

		ImGui::PopID();
	}
	ImGui::Unindent();

	static int changeCount = 0;

	ImGui::GetWindowDrawList()->AddRectFilled(
		ImGui::GetCursorScreenPos(),
		ImGui::GetCursorScreenPos() + ImVec2(m_size.x, ImGui::GetTextLineHeight()),
		ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]), ImGui::GetTextLineHeight() * 0.25f);
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::TextUnformatted("Selection");

	ImGui::BeginHorizontal("Selection Stats", ImVec2(m_size.x, 0));
	ImGui::Text("Changed %d time%s", changeCount, changeCount > 1 ? "s" : "");
	ImGui::Spring();
	if (ImGui::Button("Deselect All"))
		ed::ClearSelection();
	ImGui::EndHorizontal();
	ImGui::Indent();
	for (int i = 0; i < nodeCount; ++i) ImGui::Text("Node (%p)", selectedNodes[i].AsPointer());
	for (int i = 0; i < linkCount; ++i) ImGui::Text("Link (%p)", selectedLinks[i].AsPointer());
	ImGui::Unindent();

	if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
		for (auto& link : m_Links)
			ed::Flow(link.ID);

	if (ed::HasSelectionChanged())
		++changeCount;

	ImGui::EndChild();
}

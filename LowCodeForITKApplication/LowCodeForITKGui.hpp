#include <application.h>
#include "utilities/builders.h"
#include "utilities/widgets.h"
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include "UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"

#include "RefactoringStuff/StyleWindow.hpp"
#include "RefactoringStuff/NodesCreator.hpp"


#include <imgui_node_editor.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>


static inline ImRect ImGui_GetItemRect();

static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y);

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;





struct Link
{
	ed::LinkId ID;

	ed::PinId StartPinID;
	ed::PinId EndPinID;

	ImColor Color;

	Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId);
};

struct NodeIdLess
{
	bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const
	{
		return lhs.AsPointer() < rhs.AsPointer();
	}
};

static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);


struct BlueprintsBaseApp : public Application
{
	using Application::Application;

	void TouchNode(ed::NodeId id);

	float GetTouchProgress(ed::NodeId id);

	Node* FindNode(ed::NodeId id);

	Link* FindLink(ed::LinkId id);

	Pin* FindPin(ed::PinId id);

	bool IsPinLinked(ed::PinId id);

	bool CanCreateLink(Pin* a, Pin* b);

	void OnStart() override;

	void OnStop() override;

	ImColor GetIconColor(PinType type);;

	void DrawPinIcon(const Pin& pin, bool connected, int alpha);;

	void ShowStyleEditor(bool* show = nullptr);

	void ShowLeftPane(float paneWidth);

	void OnFrame(float deltaTime) override;

	void handleOrdinals(const ImVec2& editorMin, const ImVec2& editorMax);

	void pinDragLink(Pin*& newLinkPin, bool& createNewNode, Pin*& newNodeLinkPin);

	void handleGeneralDelete();

	void handleLinks();

	void handleNodes4();

	void handleNodes3(Pin* newLinkPin);

	void handleNodes2(Pin* newLinkPin);

	void handleNodes(ax::NodeEditor::Utilities::BlueprintNodeBuilder& builder, Pin* newLinkPin);

	void ShowLeftPaneAndSplitter();

	void popupsBullshit(Pin*& newNodeLinkPin, bool& createNewNode);

	void showFPS();

	int                  m_NextId = 1;
	const int            m_PinIconSize = 24;
	std::vector<std::unique_ptr<Node>>    m_Nodes;
	std::vector<Link>    m_Links;
	ImTextureID          m_HeaderBackground = nullptr;
	ImTextureID          m_SaveIcon = nullptr;
	ImTextureID          m_RestoreIcon = nullptr;
	const float          m_TouchTime = 1.0f;
	std::map<ed::NodeId, float, NodeIdLess> m_NodeTouchTime;
	bool                 m_ShowOrdinals = false;


	//refactor
	ed::NodeId m_contextNodeId{};
	ed::LinkId m_contextLinkId{};
	ed::PinId  m_contextPinId{};
	ed::EditorContext* m_Editor{ nullptr };

	std::shared_ptr<UniqueIDProvider> m_idProvider{ std::make_unique<SimpleIDProvider>() };
	std::unique_ptr<NodeCreator> m_nodeCreator{ std::make_unique<NodeCreator>(m_idProvider) };
	StyleWindow m_styleWindow;
};

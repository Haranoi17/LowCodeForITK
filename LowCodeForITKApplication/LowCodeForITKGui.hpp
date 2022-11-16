#include <application.h>
#include "utilities/builders.h"
#include "utilities/widgets.h"
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include "UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"

#include "RefactoringStuff/StyleWindow.hpp"
#include "RefactoringStuff/NodesCreator.hpp"
#include "RefactoringStuff/Pane.hpp"
#include "RefactoringStuff/VerticalSplitter.hpp"

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

using ax::Widgets::IconType;








class LeftPane : public Pane
{
public:
	LeftPane() {
		m_size = ImVec2{ 400,0 };
	};
	void draw() override;

private:
	StyleWindow m_styleWindow;
};

class NodeEditorPane : public Pane
{
public:
	NodeEditorPane() {
		m_size = ImVec2{ 1600,0 };
	};
	void draw() override;
};





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




inline std::vector<std::unique_ptr<Node>>    m_Nodes;
inline const int            m_PinIconSize = 24;
inline ImTextureID          m_HeaderBackground = nullptr;
inline ImTextureID          m_SaveIcon = nullptr;
inline ImTextureID          m_RestoreIcon = nullptr;
inline const float          m_TouchTime = 1.0f;
inline bool                 m_ShowOrdinals = false;
inline std::vector<Link>    m_Links;
inline bool createNewNode = false;
inline Pin* newNodeLinkPin = nullptr;
inline Pin* newLinkPin = nullptr;

float GetTouchProgress(ed::NodeId id);

Node* FindNode(ed::NodeId id);

Link* FindLink(ed::LinkId id);

Pin* FindPin(ed::PinId id);

bool IsPinLinked(ed::PinId id);

bool CanCreateLink(Pin* a, Pin* b);

void popupsBullshit(Pin*& newNodeLinkPin, bool& createNewNode);

void handleNodes4();

void handleNodes3(Pin* newLinkPin);

void handleNodes2(Pin* newLinkPin);

void handleNodes(ax::NodeEditor::Utilities::BlueprintNodeBuilder& builder, Pin* newLinkPin);

void DrawPinIcon(const Pin& pin, bool connected, int alpha);

void handleLinks();

void pinDragLink(Pin*& newLinkPin, bool& createNewNode, Pin*& newNodeLinkPin);

ImColor GetIconColor(PinType type);

void handleOrdinals(const ImVec2& editorMin, const ImVec2& editorMax);

void handleGeneralDelete();

inline std::shared_ptr<UniqueIDProvider> m_idProvider{ std::make_unique<SimpleIDProvider>() };
inline std::unique_ptr<NodeCreator> m_nodeCreator{ std::make_unique<NodeCreator>(m_idProvider) };

inline ed::NodeId m_contextNodeId{};
inline ed::LinkId m_contextLinkId{};
inline ed::PinId  m_contextPinId{};
inline ed::EditorContext* m_Editor{ nullptr };

struct BlueprintsBaseApp : public Application
{
	using Application::Application;

	void OnStart() override;

	void OnStop() override;

	void OnFrame(float deltaTime) override;
	void showFPS();

	std::shared_ptr<LeftPane> m_leftPane{ std::make_unique<LeftPane>() };
	std::shared_ptr<NodeEditorPane> m_nodeEditorPane{ std::make_unique<NodeEditorPane>() };

	std::unique_ptr<VerticalSplitter> m_verticalSplitter{ std::make_unique<VerticalSplitter>(m_leftPane, m_nodeEditorPane) };
};


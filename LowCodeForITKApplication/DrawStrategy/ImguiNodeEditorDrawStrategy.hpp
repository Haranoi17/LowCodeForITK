#pragma once
#include "DrawStrategy/Interface/DrawStrategy.hpp"

struct ImguiNodeEditorDrawStrategy : DrawStrategy
{
	void drawNode(const std::unique_ptr<Node>& node) const override;
	void drawInputPin(const std::shared_ptr<Pin>& pin) const override;
	void drawOutputPin(const std::shared_ptr<Pin>& pin) const override;
};
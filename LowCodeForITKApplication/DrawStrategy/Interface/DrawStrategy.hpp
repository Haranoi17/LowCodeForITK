#pragma once
#include "Logic/Interface/Node.hpp"
#include "Logic/Interface/Pin.hpp"

struct DrawStrategy
{
	virtual void drawNode(const std::unique_ptr<Node>& node) const = 0;
	virtual void drawInputPin(const std::shared_ptr<Pin>& pin) const = 0;
	virtual void drawOutputPin(const std::shared_ptr<Pin>& pin) const = 0;
};

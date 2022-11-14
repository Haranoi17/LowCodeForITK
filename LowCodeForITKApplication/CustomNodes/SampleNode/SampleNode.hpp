#pragma once
#include "CustomNodes/Interface/CustomNode.hpp"
#include "Identification/Identifiable.hpp"

class SampleNode final : public CustomNode, public Identifiable
{
public:
	using Identifiable::Identifiable;

	virtual void callITKStrategy();
	virtual void draw();
};
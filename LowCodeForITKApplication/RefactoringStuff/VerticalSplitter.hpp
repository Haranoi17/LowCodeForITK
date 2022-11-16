#pragma once
#include "Splitter.hpp"

class VerticalSplitter : public Splitter
{
public:
	using Splitter::Splitter;

	void split() override;
};

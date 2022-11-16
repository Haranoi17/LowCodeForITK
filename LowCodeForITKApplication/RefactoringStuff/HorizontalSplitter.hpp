#pragma once
#include "Splitter.hpp"

class HorizontalSplitter : public Splitter
{
public:
	using Splitter::Splitter;

	void split() override;
};
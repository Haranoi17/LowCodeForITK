#pragma once
#include <imgui.h>

class Resizeable
{
public:
	virtual void resize(ImVec2 size) = 0;
	virtual float getWidth() = 0;
	virtual float getHeight() = 0;
};

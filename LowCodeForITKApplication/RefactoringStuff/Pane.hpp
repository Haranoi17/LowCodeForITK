#pragma once
#include "Resizeable.hpp"

class Pane : public Resizeable
{
public:
	//Resizeable
	void resize(ImVec2 size) override { m_size = size; }
	float getWidth() override { return m_size.x; }
	float getHeight() override { return m_size.y; }

	//Pane
	virtual void draw() = 0;
protected:
	ImVec2 m_size;
};

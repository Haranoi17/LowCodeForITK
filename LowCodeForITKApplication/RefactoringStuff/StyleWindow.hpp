#pragma once

class StyleWindow
{
public:
	void drawIfNecessary();
	bool m_shouldDraw{ false };
};
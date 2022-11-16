#pragma once
#include "Resizeable.hpp"
#include <memory>

class Splitter
{
public:
	Splitter(std::shared_ptr<Resizeable> first, std::shared_ptr<Resizeable> second)
		: m_resizeables{ std::make_pair(first, second) }
	{}

	virtual void split() {}

	virtual void update() {}
protected:
	std::pair<std::shared_ptr<Resizeable>, std::shared_ptr<Resizeable>> m_resizeables;

};
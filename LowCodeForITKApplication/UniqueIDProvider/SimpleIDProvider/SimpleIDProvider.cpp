#include "SimpleIDProvider.hpp"
#include <algorithm>

IDType SimpleIDProvider::generateID()
{
	return ++m_lastId;
}

void SimpleIDProvider::freeID(IDType id)
{
	m_lastId = id;
}

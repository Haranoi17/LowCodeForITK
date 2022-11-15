#include "Identifiable.hpp"
#include <algorithm>

//Identifiable::Identifiable(std::shared_ptr<UniqueIDProvider> idPorvider) : m_idProvider{ idPorvider } {}

void Identifiable::setIdProvider(std::shared_ptr<UniqueIDProvider> idProvider)
{
	m_idProvider = idProvider;
}

Identifiable::~Identifiable()
{
	//std::ranges::for_each(m_ids, [&](IDType id) {m_idProvider->freeID(id);});
}

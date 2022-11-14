#include "Identifiable.hpp"
#include <algorithm>

Identifiable::Identifiable(std::shared_ptr<UniqueIDProvider> idPorvider) : m_idProvider{ idPorvider } {}

inline Identifiable::~Identifiable()
{
	std::ranges::for_each(m_ids, [&](IDType id) {m_idProvider->freeID(id);});
}

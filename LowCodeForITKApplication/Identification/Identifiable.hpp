#pragma once
#include "TypeAliases.hpp"
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include <memory>
#include <vector>

class Identifiable
{
public:
	Identifiable(std::shared_ptr<UniqueIDProvider> idPorvider);

	virtual ~Identifiable();

protected:
	std::shared_ptr<UniqueIDProvider> m_idProvider;
	std::vector <IDType> m_ids;
};
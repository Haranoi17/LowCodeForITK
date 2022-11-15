#pragma once
#include "TypeAliases.hpp"
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include <memory>
#include <vector>

class Identifiable
{
public:
	Identifiable() = default;
	virtual ~Identifiable();

	virtual void setIdProvider(std::shared_ptr<UniqueIDProvider> idProvider);
protected:
	std::shared_ptr<UniqueIDProvider> m_idProvider;
};
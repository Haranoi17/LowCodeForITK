#pragma once
#include "Identification/TypeAliases.hpp"

class UniqueIDProvider
{
public:
	virtual ~UniqueIDProvider() = default;
	virtual IDType generateID() = 0;
	virtual void freeID(IDType id) = 0;
};
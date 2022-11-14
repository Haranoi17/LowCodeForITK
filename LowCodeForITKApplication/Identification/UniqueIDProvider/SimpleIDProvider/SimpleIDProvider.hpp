#pragma once
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"

#include <vector>

class SimpleIDProvider final : public UniqueIDProvider
{
public:
	IDType generateID() override;
	void freeID(IDType id) override;

private:
	IDType m_lastId{ 0 };
};
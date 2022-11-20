#pragma once
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include <memory>
#include <vector>

class Pin
{
public:
	Pin(std::unique_ptr<UniqueIDProvider>& idProvider) : id{ idProvider->generateID() }
	{
	}
	IDType id;
	std::vector<std::shared_ptr<Pin>> connectedPins;
};

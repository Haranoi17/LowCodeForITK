#pragma once
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include <memory>
#include <vector>

#include <iostream>
#include <any>

class Pin
{
public:
	Pin(std::unique_ptr<UniqueIDProvider>& idProvider, std::string_view name = "anyValue") : id{ idProvider->generateID() }, name{ name.data() }
	{
	}

	virtual void calculate() { std::cout << name << " calculating pin\n"; }


	IDType id;
	std::vector<std::shared_ptr<Pin>> connectedPins;
	std::any value;
	std::string name;
};

class IntPin : public Pin
{
public:
	IntPin(std::unique_ptr<UniqueIDProvider>& idProvider, std::string_view name = "int") : Pin{ idProvider, name } {}

	void calculate() override
	{
		Pin::calculate();
		value = 1;
	}
};
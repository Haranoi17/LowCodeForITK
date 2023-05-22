#include "SampleNodeForAutomation.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

SampleNodeForAutomation::SampleNodeForAutomation(UniqueIDProvider *idProvider, std::string_view name) : Node{idProvider, name}
{
}

void SampleNodeForAutomation::calculate()
{
   
}

void SampleNodeForAutomation::populateOutputPins()
{
}

void SampleNodeForAutomation::deserialize(json data)
{
}

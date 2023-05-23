#include "SampleNodeForAutomation.hpp"
#include "Logic/Pins/DefinedPins/DefinedPins.hpp"

SampleNodeForAutomation::SampleNodeForAutomation(UniqueIDProvider *idProvider) : Node{idProvider, typeName}
{
}

void SampleNodeForAutomation::calculate()
{
   
}

void SampleNodeForAutomation::populateOutputPins()
{
}

json SampleNodeForAutomation::serialize()
{
    json serializedNode = Node::serialize();
    //Insert serialization here eg. serializedNode["value"] = value;
    return serializedNode;
}

void SampleNodeForAutomation::deserialize(json data)
{
    Node::deserialize(data);
}

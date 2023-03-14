#include "RGBANode.hpp"
#include "Pins/DefinedPins/DefinedPins.hpp"

RGBANode::RGBANode(UniqueIDProvider *idProvider, std::string_view name) : Node{idProvider, name}
{
    m_outputPins.emplace_back(std::make_unique<RGBAPin>(idProvider, this));
    rgbaPin = m_outputPins.back().get();
}
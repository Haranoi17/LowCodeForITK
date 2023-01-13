#include "Node.hpp"
#include <algorithm>

void Node::calculate()
{
    std::cout << "NODE:\n   " << name << " node calculation\n";
}

void Node::populateOutputPins()
{
    std::cout << "  " << name << " populating output pins \n";
}

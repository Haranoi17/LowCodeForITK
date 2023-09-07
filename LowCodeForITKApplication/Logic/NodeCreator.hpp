#pragma once

#include <Logic/Nodes/Node.hpp>
#include <Logic/UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp>
#include <memory>

class NodeCreator
{
  public:
    virtual ~NodeCreator()                                                            = default;
    virtual std::unique_ptr<Node> createFullyFunctional(UniqueIDProvider *idProvider) = 0;
    virtual std::unique_ptr<Node> createMocked()                                      = 0;
};

template <typename NodeType>
class SpecificNodeCreator : public NodeCreator
{
    inline static std::unique_ptr<UniqueIDProvider> mockIdProvider{std::make_unique<SimpleIDProvider>()};

  public:
    ~SpecificNodeCreator() = default;
    std::unique_ptr<Node> createFullyFunctional(UniqueIDProvider *idProvider) override
    {
        return std::make_unique<NodeType>(idProvider);
    };

    std::unique_ptr<Node> createMocked() override
    {
        return std::make_unique<NodeType>(mockIdProvider.get());
    };
};

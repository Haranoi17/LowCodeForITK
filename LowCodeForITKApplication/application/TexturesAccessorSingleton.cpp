#include "TexturesAccessorSingleton.hpp"

TexturesAccessorSingleton *TexturesAccessorSingleton::instance()
{
    static TexturesAccessorSingleton instance;

    return &instance;
}

ImTextureID TexturesAccessorSingleton::getNodeHeaderTexture()
{
    if (!m_nodeHeaderTexture)
    {
        auto texturesOperations = TexturesOperationsProxySingleton::instance();
        m_nodeHeaderTexture     = texturesOperations->LoadTexture("data/textures/BlueprintBackground.png");
    }

    return m_nodeHeaderTexture.value();
}
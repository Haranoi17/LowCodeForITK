#pragma once
#include "TexturesOperationsProxySingleton.hpp"
#include <optional>

struct TexturesAccessorSingleton
{
    static TexturesAccessorSingleton *instance();

    ImTextureID getNodeHeaderTexture();

  private:
    TexturesAccessorSingleton() = default;
    std::optional<ImTextureID> m_nodeHeaderTexture;
};
#pragma once
#include "TextureOperator.hpp"
#include <imgui.h>

struct TexturesRepository
{
    virtual ~TexturesRepository() = default;

    virtual void        loadTextures(TextureOperator *texturesOperator) = 0;
    virtual ImTextureID getNodeHeaderTexture()                          = 0;
};
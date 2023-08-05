#pragma once
#include "imgui.h"

struct TextureOperator
{
    virtual ~TextureOperator()                                                 = default;

    virtual ImTextureID LoadTexture(const char *path) = 0;
    virtual ImTextureID CreateTexture(const void *data, int width, int height) = 0;
    virtual void AddTextureToDestroy(ImTextureID texture) = 0;
    virtual void DestroyTextures() = 0;
    virtual int GetTextureWidth(ImTextureID texture) = 0;
    virtual int GetTextureHeight(ImTextureID texture) = 0;
};

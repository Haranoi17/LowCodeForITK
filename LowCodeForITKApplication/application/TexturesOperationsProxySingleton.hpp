#pragma once
#include "application.h"
#include <vector>

struct TexturesOperationsProxySingleton
{
    static TexturesOperationsProxySingleton *instance(Application *application = nullptr);

    ImTextureID LoadTexture(const char *path);

    ImTextureID CreateTexture(const void *data, int width, int height);

    void AddTextureToDestroy(ImTextureID texture);
    void DestroyTextures();

    int GetTextureWidth(ImTextureID texture);

    int GetTextureHeight(ImTextureID texture);

  private:
    TexturesOperationsProxySingleton(Application *application);
    Application *m_application;

    std::vector<ImTextureID> texturesToDestroy;
};

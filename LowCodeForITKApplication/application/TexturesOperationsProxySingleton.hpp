#pragma once
#include "application.h"

struct TexturesOperationsProxySingleton
{
    static TexturesOperationsProxySingleton *instance(Application *application = nullptr);

    ImTextureID LoadTexture(const char *path);

    ImTextureID CreateTexture(const void *data, int width, int height);

    void DestroyTexture(ImTextureID texture);

    int GetTextureWidth(ImTextureID texture);

    int GetTextureHeight(ImTextureID texture);

  private:
    TexturesOperationsProxySingleton(Application *application);
    Application *m_application;
};

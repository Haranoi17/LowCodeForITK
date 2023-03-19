#include "Application/TexturesOperationsProxySingleton.hpp"

TexturesOperationsProxySingleton *TexturesOperationsProxySingleton::instance(Application *application)
{
    static TexturesOperationsProxySingleton instance{application};

    return &instance;
}

TexturesOperationsProxySingleton::TexturesOperationsProxySingleton(Application *application)
    : m_application{application}
{
}

ImTextureID TexturesOperationsProxySingleton::LoadTexture(const char *path)
{
    return m_application->LoadTexture(path);
}

ImTextureID TexturesOperationsProxySingleton::CreateTexture(const void *data, int width, int height)
{
    return m_application->CreateTexture(data, width, height);
}

void TexturesOperationsProxySingleton::DestroyTexture(ImTextureID texture)
{
    m_application->DestroyTexture(texture);
}

int TexturesOperationsProxySingleton::GetTextureWidth(ImTextureID texture)
{
    return m_application->GetTextureWidth(texture);
}

int TexturesOperationsProxySingleton::GetTextureHeight(ImTextureID texture)
{
    return m_application->GetTextureHeight(texture);
}

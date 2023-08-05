#include "ApplicationTextureOperator.hpp"

ApplicationTextureOperator::ApplicationTextureOperator(Application *application) : application{application}
{
}

ImTextureID ApplicationTextureOperator::LoadTexture(const char *path)
{
    return application->LoadTexture(path);
}

ImTextureID ApplicationTextureOperator::CreateTexture(const void *data, int width, int height)
{
    return application->CreateTexture(data, width, height);
}

void ApplicationTextureOperator::AddTextureToDestroy(ImTextureID texture)
{
    texturesToDestroy.emplace_back(texture);
}

void ApplicationTextureOperator::DestroyTextures()
{
    for (const auto texture : texturesToDestroy)
    {
        application->DestroyTexture(texture);
    }

    texturesToDestroy.clear();
}

int ApplicationTextureOperator::GetTextureWidth(ImTextureID texture)
{
    return application->GetTextureWidth(texture);
}

int ApplicationTextureOperator::GetTextureHeight(ImTextureID texture)
{
    return application->GetTextureHeight(texture);
}

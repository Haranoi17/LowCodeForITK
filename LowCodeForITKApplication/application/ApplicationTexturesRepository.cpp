#include "ApplicationTexturesRepository.hpp"

void ApplicationTexturesRepository::loadTextures(TextureOperator *textureOperator)
{
    nodeHeaderTexture = textureOperator->LoadTexture("data/textures/BlueprintBackground.png");
}

ImTextureID ApplicationTexturesRepository::getNodeHeaderTexture()
{
    return nodeHeaderTexture;
}
#include <Application/Drawing/TexturesRepository.hpp>
#include <optional>

class ApplicationTexturesRepository : public TexturesRepository
{
    void        loadTextures(TextureOperator *textureOperator) override;
    ImTextureID getNodeHeaderTexture() override;

  private:
	ImTextureID nodeHeaderTexture;
};
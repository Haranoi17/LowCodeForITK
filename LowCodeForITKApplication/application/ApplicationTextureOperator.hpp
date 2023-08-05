#include <Application/Drawing/TextureOperator.hpp>
#include <application.h>

#include <vector>

class ApplicationTextureOperator : public TextureOperator
{
  public:
    ApplicationTextureOperator(Application *application);
    ImTextureID LoadTexture(const char *path) override;
    ImTextureID CreateTexture(const void *data, int width, int height) override;
    void        AddTextureToDestroy(ImTextureID texture) override;
    void        DestroyTextures() override;
    int         GetTextureWidth(ImTextureID texture) override;
    int         GetTextureHeight(ImTextureID texture) override;

  private:
    Application             *application;
    std::vector<ImTextureID> texturesToDestroy;
};
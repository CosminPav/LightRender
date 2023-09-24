#include "TextureManager.h"
#include "Texture.h"
#include <memory>
#include <exception>

TexturePtr TextureManager::CreatTextureFromFile(const wchar_t* FilePath)
{
    return std::static_pointer_cast<Texture>(CreateResourceFromFile(FilePath));
}

Resource* TextureManager::CreateResourceFromFileConcrete(const wchar_t* FilePath)
{
    Texture* Tex = nullptr;
    try {
        Tex = new Texture(FilePath);
    }
    catch (...) {
        throw std::exception("Texture has not been created");
    }
  
    return Tex;
}

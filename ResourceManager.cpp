#include "ResourceManager.h"

#include <filesystem>

ResourcePtr ResourceManager::CreateResourceFromFile(const wchar_t* FileName)
{
    std::wstring FullPath = std::filesystem::absolute(FileName);

    auto It = Resources.find(FullPath);
    if (It != Resources.end()) {
        return It->second;
    }
    
    //If the resource deos not exist, will be created
    Resource* RawResource = CreateResourceFromFileConcrete(FullPath.c_str());
    if (RawResource) {
        ResourcePtr Resource(RawResource);
        Resources[FullPath] = Resource;
        return Resource;
    }

    return nullptr;
}

ResourceManager::~ResourceManager()
{
}

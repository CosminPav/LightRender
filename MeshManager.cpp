#include "MeshManager.h"
#include "Mesh.h"

MeshPtr MeshManager::CreateMeshFromFile(const wchar_t* FilePath)
{
    return std::static_pointer_cast<Mesh>(CreateResourceFromFile(FilePath));
}

Resource* MeshManager::CreateResourceFromFileConcrete(const wchar_t* FilePath)
{
    Mesh* mesh = nullptr;
    try {
        mesh = new Mesh(FilePath);
    }
    catch (...) {
        throw std::exception("Mesh has not been created");
    }

    return mesh;
}

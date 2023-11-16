#pragma once
#include"ResourceManager.h"
#include "Prerequisites.h"

class MeshManager:public ResourceManager
{
public:
	MeshManager() :ResourceManager()
	{}

	MeshPtr CreateMeshFromFile(const wchar_t* FilePath);

	~MeshManager() = default;
protected:
	Resource* CreateResourceFromFileConcrete(const wchar_t* FilePath) override;
};


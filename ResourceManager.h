#pragma once
#include <unordered_map>
#include <string>
#include "Prerequisites.h"
#include "Resource.h"
#include <memory>

class ResourceManager
{

	std::unordered_map<std::wstring, ResourcePtr> Resources;

public:
	ResourceManager() {}

	//Create resource
	ResourcePtr CreateResourceFromFile(const wchar_t* FileName);

	virtual ~ResourceManager();
protected:
	virtual Resource* CreateResourceFromFileConcrete(const wchar_t* FilePath) = 0;
};


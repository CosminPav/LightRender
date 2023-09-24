#pragma once
#include <string>

class Resource
{
public:
	Resource(const wchar_t* FullPath);
	virtual ~Resource();

protected:
	std::wstring mFullPath;
};


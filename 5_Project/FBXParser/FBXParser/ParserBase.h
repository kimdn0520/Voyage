#pragma once
#include "FBXParserDLL.h"
#include "ParserData.h"

class ParserBase
{
public:
	ParserBase();
	virtual ~ParserBase();

public:
	static FBXParser_DLL std::shared_ptr<ParserBase> Create();

	FBXParser_DLL virtual std::shared_ptr<NoobData::FbxModel> LoadFbx(const std::wstring& path) abstract;

	FBXParser_DLL virtual void Release() abstract;
};


#include "pch.h"
#include "ParserBase.h"
#include "FBXLoader.h"

ParserBase::ParserBase()
{
}

ParserBase::~ParserBase()
{
}

FBXParser_DLL std::shared_ptr<ParserBase> ParserBase::Create()
{
    std::shared_ptr<ParserBase> newFbxLoader;

    newFbxLoader = std::make_shared<FBXLoader>();

    return newFbxLoader;
}

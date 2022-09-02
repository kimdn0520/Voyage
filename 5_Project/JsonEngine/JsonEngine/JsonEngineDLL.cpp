#include "pch.h"
#include "JsonEngineDLL.h"
#include "Product.h"
#include "Products.h"


static IJson* GJsonEngine = nullptr;

_JSONDLL bool DLLJson::DeserializeFromFile(const std::string& filePath)
{
	bool isResult = false;
	if (GJsonEngine == nullptr)
	{
		GJsonEngine = new JsonBase;
		isResult = GJsonEngine->DeserializeFromFile(filePath);
	}
	else
	{
		_TRACE("");
	}
	return  isResult;
}

_JSONDLL bool DLLJson::DeserializeFromData(const std::string& data)
{
	return _JSONDLL bool();
}

_JSONDLL bool DLLJson::SerializeToFile(const std::string& filePath)
{
	return _JSONDLL bool();
}

_JSONDLL std::string DLLJson::Serialize()
{
	return _JSONDLL std::string();
}

bool DLLJson::Deserialize(const std::string& string)
{
	return false;
}

bool DLLJson::Deserialize(const rapidjson::Value& obj)
{
	return false;
}

bool DLLJson::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer)
{
	return false;
}

bool DLLJson::InitDocument(const std::string& strings, rapidjson::Document& doc)
{
	return false;
}

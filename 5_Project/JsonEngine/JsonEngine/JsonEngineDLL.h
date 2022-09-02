#pragma once
#include "JsonDLLDefine.h"

class Product;


namespace DLLJson
{
	_JSONDLL bool DeserializeFromFile(const std::string& filePath);
	_JSONDLL bool DeserializeFromData(const std::string& data);
	_JSONDLL bool SerializeToFile(const std::string& filePath);

	_JSONDLL  std:: string Serialize();
	_JSONDLL  bool Deserialize(const std::string& string);
	_JSONDLL  bool Deserialize(const rapidjson::Value& obj);
	_JSONDLL  bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer);
	_JSONDLL bool InitDocument(const std::string& strings, rapidjson::Document& doc);
}
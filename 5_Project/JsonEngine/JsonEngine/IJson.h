#pragma once

using namespace std;

__interface IJson
{
public:
	virtual	bool DeserializeFromFile(const string& filePath) abstract;
	virtual	bool DeserializeFromData(const string& data)abstract;
	virtual	bool SerializeToFile(const string& filePath)abstract;
	
	virtual string Serialize() const abstract;
	virtual bool Deserialize(const std::string& string)abstract;
	virtual bool Deserialize(const rapidjson::Value& obj) = 0;
	virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer)const = 0;
};
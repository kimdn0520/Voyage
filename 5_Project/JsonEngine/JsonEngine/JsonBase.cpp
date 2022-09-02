#include "pch.h"
#include "JsonBase.h"

string JsonBase::Serialize() const
{
	rapidjson::StringBuffer stringBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);
	if (Serialize(&writer))
	{
		return stringBuffer.GetString();
	}
	return "";
}

 bool JsonBase::Deserialize(const std::string& string)
{
	rapidjson::Document doc;
	if (!InitDocument(string, doc))
	{
		return false;
	}
	Deserialize(doc);
	return true;
}

 bool JsonBase::DeserializeFromFile(const std::string& filePath)
{
	std::ifstream f(filePath);
	std::stringstream buffer;
	buffer << f.rdbuf();
	f.close();

	return Deserialize(buffer.str());
}

 bool JsonBase::DeserializeFromData(const std::string& data)
{
	std::stringstream buffer;
	buffer << data.c_str();

	return Deserialize(buffer.str());
}

 bool JsonBase::SerializeToFile(const std::string& filePath)
{
	std:: ofstream f(filePath);
	std::string string = Serialize();
	f << string;
	f.flush();
	f.close();
	return true;
}

bool JsonBase::InitDocument(const std::string& string, rapidjson::Document& doc)
{
	if (string.empty())
	{
		return false;
	}

	std:: string validJson(string);
	return doc.Parse(validJson.c_str()).HasParseError() ? true : false;
}
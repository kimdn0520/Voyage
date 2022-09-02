#pragma once
#include "JsonBase.h"


class Products : public JsonBase
{
public:
	virtual ~Products() {};
	virtual bool Deserialize(const std::string& string);
	bool DeserializeFile(const std::string& filePath);

	// Getters/Setters.
	std::list<Product>& ProductsList() { return _products; }
public:
	virtual void DataSetup(const rapidjson::Value& obj) abstract;
	virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
	virtual bool Deserialize(const rapidjson::Value& obj) { return false; };
private:
	std::list<Product> _products;
};


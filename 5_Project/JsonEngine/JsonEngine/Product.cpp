#include "pch.h"
#include "Product.h"

Product::Product():_id(0),_script(0)
{
}

Product::~Product()
{
}

bool Product::Deserialize(const rapidjson::Value& obj)
{
	Id(obj["Id"].GetInt());
	Script(obj["FileName"].GetString());
	return true;
}

bool Product::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
	writer->StartObject();

	writer->String("Id");
	writer->Int(_id);
	
	writer->String("Script");
	writer->String(_script.c_str());

	writer->EndObject();

	return true;
}

#include "pch.h"
#include "Products.h"
#include "Product.h"

bool Products::Deserialize(const std::string& string)
{
	rapidjson::Document doc;
	
	int i = 0;
	while (string[i] != '[')
	{
		i++;
	}

	std::string str = string.substr(i);

	if (InitDocument(str,doc))
	{
		return false;
	}

	if (!doc.IsArray())
	{
		return false;
	}

	for (rapidjson::Value::ConstValueIterator itr = doc.Begin(); itr != doc.End(); ++itr)
	{
		//Product p;
		//p.Deserialize(*itr);
		//mProducts.push_back(p);

		DataSetup(*itr);
	}

	return true;
}

bool Products::DeserializeFile(const std::string& filePath)
{
	return DeserializeFromFile(filePath);
}

bool Products::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
	writer->StartArray();

	for (std::list<Product>::const_iterator it = _products.begin(); it != _products.end(); it++)
	{
		(*it).Serialize(writer);
	}

	writer->EndArray();

	return true;
}
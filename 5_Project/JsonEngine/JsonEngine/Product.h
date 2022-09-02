#pragma once
#include "JsonBase.h"


class Product : public JsonBase
{
public:
	Product();
	virtual ~Product();

	virtual bool Deserialize(const rapidjson::Value& obj);
	virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;

	//// Getters/Setters.
	int Id() const { return _id; }
	void Id(int id) { _id = id; }

	const std::string& Script() const { return _script; }
	void Script(const std::string& Script) { _script = Script; }


private:
	int _id;
	std:: string _script;

};


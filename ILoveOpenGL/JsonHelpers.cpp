#include "JsonHelpers.h"
#include <string>

bool SetValue(rapidjson::Value& val, float& toSet)
{
	if (val.IsFloat())
	{
		toSet = val.GetFloat();
		return true;
	}
	return false;
}

bool SetValue(rapidjson::Value& val, std::string& toSet)
{
	if (val.IsString())
	{
		toSet = val.GetString();
		return true;
	}
	return false;
}

bool SetValue(rapidjson::Value& val, int& toSet)
{
	if (val.IsInt())
	{
		toSet = val.GetInt();
		return true;
	}
	return false;
}
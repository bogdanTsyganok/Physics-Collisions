#pragma once

// json
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <string>

bool SetValue(rapidjson::Value& val, float& toSet);


bool SetValue(rapidjson::Value& val, std::string& toSet);

bool SetValue(rapidjson::Value& val, int& toSet);
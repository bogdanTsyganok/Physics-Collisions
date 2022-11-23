#pragma once
/*
* Author:		Bogdan Tsyganok, 0886354
* Description:	Defines a class to save and load configuration files
*/

// std
#include <string>
#include <map>
#include <sstream>
#include <iostream>

#include "PinballToys.h"
#include "JsonHelpers.h"
#include <vector>

typedef std::map<std::string, cPinballToy> objectMap;

class cObjectsLoader
{
public:
	static void readProjectileConfig(std::string fileName, std::map<std::string, cPinballToy>& objectsOut);
};

/*
* filePath: Provide without document extension (.xml/.json)
* prefOut: Actual return value
* Returns whether action is successful
* Only works with JSON files
*/
//bool ReadPrefsFromJSON(const std::string& filePath, sPreferences& prefOut)
//{
//
//	FILE* fp = 0;
//	fopen_s(&fp, (filePath + ".json").c_str(), "rb"); // non-Windows use "r"
//
//	char readBuffer[65536];
//	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
//
//	rapidjson::Document d;
//	d.ParseStream(is);
//
//	if (d["Preferences"].IsObject())
//	{
//		const rapidjson::Value& prefs = d["Preferences"];
//		if (prefs["Language"].IsString()
//			&& prefs["Graphics"].IsString()
//			&& prefs["Sound"].IsInt()
//			&& prefs["MCName"].IsString())
//		{
//			prefOut.currentGraphics = prefs["Graphics"].GetString();
//			prefOut.currentLanguage = prefs["Language"].GetString();
//			prefOut.currentMCName = prefs["MCName"].GetString();
//			prefOut.currentSound = prefs["Sound"].GetInt();
//		}
//		else
//		{
//			fclose(fp);
//			return false;
//		}
//	}
//
//	fclose(fp);
//	return true;
//}
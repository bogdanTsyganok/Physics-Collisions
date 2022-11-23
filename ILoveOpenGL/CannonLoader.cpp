#include "CannonLoader.h"



void cCannonLoader::readCannonConfig(std::string fileName, sCannonConfig& configOut)
{
	FILE* fp = 0;
	fopen_s(&fp, (fileName).c_str(), "rb");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	if (d.IsObject())
	{
		SetValue(d.GetObject()["MaxPitch"], configOut.maxPitch);
		SetValue(d.GetObject()["MinPitch"], configOut.minPitch);
		SetValue(d.GetObject()["MaxYaw"], configOut.maxYaw);
		SetValue(d.GetObject()["MinYaw"], configOut.minYaw);
		SetValue(d.GetObject()["InitPitch"], configOut.initPitch);
		SetValue(d.GetObject()["InitYaw"], configOut.initYaw);
	}


	fclose(fp);
}

void cCannonLoader::readArenaConfig(std::string fileName, sArenaConfig& configOut)
{
	FILE* fp = 0;
	fopen_s(&fp, (fileName).c_str(), "rb");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	if (d.IsObject())
	{
		SetValue(d.GetObject()["Offset"], configOut.offset);
		SetValue(d.GetObject()["Pitch"], configOut.pitch);
		if (d["Spheres"].IsArray())
		{
			rapidjson::GenericArray<false, rapidjson::Value> spheres = d["Spheres"].GetArray();
			for (int k = 0; k < spheres.Size(); k++)
			{
				sSphere tempShere;
				SetValue(spheres[k]["r"], tempShere.r);
				SetValue(spheres[k]["x"], tempShere.x);
				SetValue(spheres[k]["y"], tempShere.y);
				SetValue(spheres[k]["z"], tempShere.z);
				configOut.spheres.push_back(tempShere);
			}
		}
	}


	fclose(fp);
}

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
#include "ObjectLoader.h"



void cObjectsLoader::readProjectileConfig(std::string fileName, std::map<std::string, cPinballToy>& objectsOut)
{
	FILE* fp = 0;
	fopen_s(&fp, (fileName).c_str(), "rb");

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	if (d.IsArray())
	{
		rapidjson::GenericArray<false, rapidjson::Value> n = d.GetArray();
		for (int i = 0; i < n.Size(); ++i)
		{
			if (n[i].IsObject())
			{
				cPinballToy tempConfig;
				std::string name;
				SetValue((n[i]["Name"]), name);

				if (n[i]["Spheres"].IsArray())
				{

					rapidjson::GenericArray<false, rapidjson::Value> spheres = n[i]["Spheres"].GetArray();
					for (int k = 0; k < spheres.Size(); ++k)
					{
						float x, y, z, r;
						SetValue(spheres[k]["x"], x);
						SetValue(spheres[k]["y"], y);
						SetValue(spheres[k]["z"], z);
						SetValue(spheres[k]["r"], r);
						tempConfig.addObject(glm::vec4(x, y, z, r));
					}
				}

				if (n[i]["Constraints"].IsArray())
				{

					rapidjson::GenericArray<false, rapidjson::Value> constraints = n[i]["Constraints"].GetArray();
					for (int k = 0; k < constraints.Size(); ++k)
					{
						int x, y;
						SetValue(constraints[k]["x"], x);
						SetValue(constraints[k]["y"], y);
						tempConfig.addRodConstraint(x, y);
					}
				}

				objectsOut.insert(std::make_pair(name, tempConfig));
			}
		}
	}



	fclose(fp);
}

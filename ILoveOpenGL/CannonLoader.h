#pragma once
/*
* Author:		Bogdan Tsyganok, 0886354
* Description:	Defines a class to save and load configuration files
*/

// std
#include <string>
#include <vector>
#include <sstream>
#include <iostream>


#include "JsonHelpers.h"

struct sCannonConfig
{
	float minPitch;
	float maxPitch;
	float minYaw;
	float maxYaw;
	float initPitch;
	float initYaw;
};

struct sSphere
{
	float r;
	float x, y, z;
};

struct sArenaConfig
{
	float offset;
	float pitch;
	std::vector<sSphere> spheres;
};

class cCannonLoader
{
public:
	static void readCannonConfig(std::string fileName, sCannonConfig& configOut);

	static void readArenaConfig(std::string fileName, sArenaConfig& configOut);
};
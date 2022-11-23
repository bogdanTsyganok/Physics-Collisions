#pragma once
#include <string>
#include "glmIncludes.h"

struct NVM
{
	std::string command;
	glm::vec4 floatData;
	std::string stringData;
};

enum class Sender { Mesh, Particle };

class iGameObjectMediator
{
public:
	virtual bool Notify(Sender sender, NVM data) = 0;
};
#pragma once

#include "GameObject.h"
#include "particleContactGenerators.h"
#include "iParticleContactGenerator.h"

class cArena
{
    std::vector<iParticleContactGenerator*> mContactGenerators;
    std::vector<cGameOjbect*> mObjects;
public:
	cArena() = delete;
	cArena(float offset, float angle = 0.f);

	void addSphere(glm::vec3 pos, float radius);

	std::vector<iParticleContactGenerator*> getGenerators();
	std::vector<cGameOjbect*> getObjects();
};
#pragma once

#include "GameObject.h"
#include "ParicleWorld.h"
#include "PinballToys.h"

class cPinballObjectBuilder
{
public:
	void recolourAtDistance(int index);

	void createAtOffset(glm::vec3 offset, cPinballToy blueprint, std::vector<cGameOjbect*>& meshes, cParticleWorld* particleWorld, std::vector<cParticleRodConstraint*>& constraintsOut, iParticleForceGenerator* gravity = NULL);
	
	std::vector<std::vector<cGameOjbect*>> sphereCollections;

	void applyForceToToy(glm::vec3 force, int index);
};
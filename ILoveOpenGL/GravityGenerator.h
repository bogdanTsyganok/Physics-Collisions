#pragma once

/*
* author:	Bogdan Tsyganok, 0886354
* purpose:	simple gravity generator
*/

#include "iParticleForceGenerator.h"

class cParticleGravityGenerator : public iParticleForceGenerator
{
	glm::vec3 gravityAcceleration;
public:
	cParticleGravityGenerator(const glm::vec3& gravityAcceleration);

	//Force creating using starting acceleration
	cParticleGravityGenerator() = delete;
	cParticleGravityGenerator(cParticleGravityGenerator& other) = delete;
	cParticleGravityGenerator& operator=(cParticleGravityGenerator& other) = delete;

	virtual ~cParticleGravityGenerator();
	virtual void Update(cParticle* particle, float deltaTime);
};
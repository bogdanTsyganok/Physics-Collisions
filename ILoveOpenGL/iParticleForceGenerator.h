#pragma once
/*
* author:	Bogdan Tsyganok, 0886354
* purpose:	abstract force generator
*/
#include "glmIncludes.h"
#include "Particle.h"

class iParticleForceGenerator
{
public:
	virtual ~iParticleForceGenerator() {}

	virtual void Update(cParticle* particle, float deltaTime) = 0;
};
#pragma once

/*
* author:	--
* purpose:	Registers forceGenerators with particles, applies all forces with one method call
*/

#include "Particle.h"
#include "iParticleForceGenerator.h"
#include <vector>
#include <map>

class cParticleForceRegistry
{
	typedef std::multimap<cParticle*, iParticleForceGenerator*> registryType;
	typedef std::multimap<cParticle*, iParticleForceGenerator*>::iterator registryIterator;
	typedef std::pair<cParticle*, iParticleForceGenerator*> registryPair;
	typedef std::pair<registryIterator, registryIterator> registryRange;

	registryType registry;
public:
	cParticleForceRegistry();
	~cParticleForceRegistry();

	cParticleForceRegistry(cParticleForceRegistry& other) = delete;
	cParticleForceRegistry& operator=(cParticleForceRegistry& other) = delete;

	void Register(cParticle* particle, iParticleForceGenerator* generator);
	void Deregister(cParticle* particle, iParticleForceGenerator* generator);
	void Deregister(cParticle* particle);
	void Deregister(iParticleForceGenerator* generator);

	void UpdateForces(float deltaTime);


};
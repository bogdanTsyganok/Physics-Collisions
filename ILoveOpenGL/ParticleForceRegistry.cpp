#include "ForceRegistry.h"


cParticleForceRegistry::cParticleForceRegistry()
{

}
cParticleForceRegistry::~cParticleForceRegistry()
{

}

void cParticleForceRegistry::Register(cParticle* particle, iParticleForceGenerator* generator)
{
	registry.insert(std::make_pair(particle, generator));
}

void cParticleForceRegistry::Deregister(cParticle* particle, iParticleForceGenerator* generator)
{
	registryRange range = registry.equal_range(particle);
	for (registryIterator it = range.first; it != range.second; )
	{
		registryIterator eraseIt = it++;
		if (eraseIt->second == generator)
		{
			registry.erase(eraseIt);
		}
	}
}

void cParticleForceRegistry::Deregister(cParticle* particle)
{
	registry.erase(particle);
}

void cParticleForceRegistry::Deregister(iParticleForceGenerator* generator)
{
	for (registryIterator it = registry.begin(); it != registry.end(); )
	{
		registryIterator eraseIt = it++;
		if (eraseIt->second == generator)
		{
			registry.erase(eraseIt);
		}
	}
}

void cParticleForceRegistry::UpdateForces(float deltaTime)
{
	for (registryIterator it = registry.begin(); it != registry.end(); it++)
	{
		// generator->Updated(particle, deltaTime);
		it->second->Update(it->first, deltaTime);
	}
}
#pragma once

/*
* author:	Bogdan Tsyganok, 0886354
* purpose:	"Particle Manager", holds, manages, cleans particles
*/

#include "Particle.h"
#include "glmIncludes.h"
#include "ForceRegistry.h"
#include "iParticleContactGenerator.h"
#include "ParticleContactResolver.h"
#include <vector>

class cParticleWorld
{
	cParticleForceRegistry* forceRegistry;
	// The list of contacts.
	cParticleContact* mContacts;
	// The maximum number of contacts allowed.
	size_t mMaxContacts;
	// All of the contact generators.
	std::vector<iParticleContactGenerator*> mContactGenerators;
	// The particle contact resolver.
	cParticleContactResolver mResolver;
	
	size_t GenerateContacts();
	// The world should calculate the number of iterations
	// to give the contact resolver each frame.
	bool mCalculateIterations;
public:
	std::vector<cParticle*> particles;
	cParticleWorld(int maxContacts, int iterations);
	virtual ~cParticleWorld();

	bool AddParticle(cParticle* particle);
	bool RemoveParticle(cParticle* particle);

	void TimeStep(float deltaTime);

	void IntegrateParticles(float deltaTime);

	void ClearDeadParticles();

	glm::vec3 DoVectorMath(int a, int b, int c, int mult);

	cParticleForceRegistry* GetForceRegistry() const;

	bool AddContactGenerator(iParticleContactGenerator* generator);
};
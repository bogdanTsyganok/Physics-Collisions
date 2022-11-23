#include "ParicleWorld.h"
#include <iostream>

cParticleWorld::cParticleWorld(int maxContacts, int iterations) : mResolver(iterations)
{
	mContacts = new cParticleContact[maxContacts];
	mCalculateIterations = (iterations == 0);
	this->mMaxContacts = maxContacts;
	forceRegistry = new cParticleForceRegistry();
}
cParticleWorld::~cParticleWorld()
{
	delete forceRegistry;
	delete[] mContacts;
}

bool cParticleWorld::AddParticle(cParticle* particle)
{
	if (!particle)
	{
		return false; // nothing was added
	}
	std::vector<cParticle*>::iterator itParticle = std::find(particles.begin(), particles.end(), particle);
	if (itParticle == particles.end())
	{
		// Not there! add it.
		particles.push_back(particle);
		return true;
	}
	return false; // nothing was added.
}

bool cParticleWorld::RemoveParticle(cParticle* particle)
{
	//check
	if (!particle)
	{
		return false; // nothing was removed
	}
	//unhook
	forceRegistry->Deregister(particle);
	
	//find
	std::vector<cParticle*>::iterator itParticle = std::find(particles.begin(), particles.end(), particle);
	if (itParticle == particles.end())
	{
		return false; // nothing was removed.
	}
	//delete
	particles.erase(itParticle);
	return true; // found it. removed it.
}

void cParticleWorld::TimeStep(float deltaTime)
{
	// 1) Update Force Generators
	forceRegistry->UpdateForces(deltaTime);
	//No need to do that YET

	// 2) Integrate the particles
	IntegrateParticles(deltaTime);
	// 3) Generate contacts
	size_t numContactsGenerated = GenerateContacts();

	// 4) Resolve contacts
	if (numContactsGenerated)
	{
		if (mCalculateIterations)
		{
			mResolver.SetIterations(numContactsGenerated * 2);
		}
		mResolver.ResolveContacts(mContacts, numContactsGenerated, deltaTime);
	}

}

size_t cParticleWorld::GenerateContacts()
{
	size_t limit = mMaxContacts;
	cParticleContact* nextContact = mContacts;

	for (iParticleContactGenerator* g : mContactGenerators)
	{
		size_t used = g->AddContact(nextContact, limit, particles);
		limit -= used;
		nextContact += used;

		// We've run out of contacts to fill. This means we're missing collisions.
		if (limit <= 0)
		{
			std::cout << __FUNCSIG__ << " -> TOO MANY CONTACTS!" << std::endl;
			break;
		}
	}

	return mMaxContacts - limit;
}

void cParticleWorld::IntegrateParticles(float deltaTime)
{
	// loop over our particles and tell them to integrate
	std::vector<cParticle*>::iterator itParticle = particles.begin();
	std::vector<cParticle*> children;
	while (itParticle != particles.end())
	{
		(*itParticle)->Integrate(deltaTime);
		if (!(*itParticle)->IsAlive())
		{
			(*itParticle)->GenerateChildren(children);
		}
		itParticle++;
	}
	particles.insert(particles.end(), children.begin(), children.end());
}

void cParticleWorld::ClearDeadParticles()

{
	//Dispose of dead particles
	std::vector<cParticle*>::iterator currentParticle = particles.begin();
	std::vector<cParticle*> toDelete;
	while (currentParticle != particles.end())
	{
		if (!(*currentParticle)->IsAlive())
		{
			toDelete.push_back(*currentParticle);
		}
		currentParticle++;
	}

	for (cParticle* deletingIt : toDelete)
	{
		RemoveParticle(deletingIt);
	}
}

glm::vec3 cParticleWorld::DoVectorMath(int a, int b, int c, int mult)
{
	return glm::vec3(a * mult, b * mult, c* mult);
}

cParticleForceRegistry* cParticleWorld::GetForceRegistry() const
{
	return forceRegistry;
}

bool cParticleWorld::AddContactGenerator(iParticleContactGenerator* generator)
{
	// 1) Null check, because you can't add nothing.
	if (!generator)
	{
		return false;
	}
	// 2) Check if we have it
	//     if no:  Add it. Then return true to indicate that it was added.
	//     if yes: Return false to indicate that nothing was added.
	std::vector<iParticleContactGenerator*>::iterator itGenerator = std::find(mContactGenerators.begin(), mContactGenerators.end(), generator);
	if (itGenerator == mContactGenerators.end())
	{
		// Not there! Add it then return true;
		mContactGenerators.push_back(generator);
		return true;
	}
	return false; // nothing was added.
}


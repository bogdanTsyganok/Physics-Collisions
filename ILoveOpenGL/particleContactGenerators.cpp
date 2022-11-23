//name:	Bogdan Tsyganok, 0886354

#include "particleContactGenerators.h"

cPlaneContactGenerator::cPlaneContactGenerator(glm::vec3 normal, float offset)
{
	mNormal = normal;
	mOffset = offset;
}

size_t cPlaneContactGenerator::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
{
	size_t numContactsCreated = 0;
	for (std::vector<cParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		glm::vec3 pos = (*it)->GetPosition();
		float distance = (glm::dot(pos, mNormal) - mOffset);
		bool intersect = distance <= (*it)->getRadius();
		if (intersect)
		{
			contact->mContactNormal = mNormal;
			contact->mParticle0 = *it;
			contact->mParticle1 = nullptr;
			contact->mPenetration = abs(distance - (*it)->getRadius());
			contact->mRestitution = 0.6f;
			contact++;
			numContactsCreated++;
		}
		if (numContactsCreated >= limit)
		{
			return numContactsCreated;
		}
	}
	return numContactsCreated;
}

cSphereContactGenerator::cSphereContactGenerator()
{
}

size_t cSphereContactGenerator::AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const
{
	size_t numContactsCreated = 0;
	for (std::vector<cParticle*>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		if ((*it)->getShape() == Sphere)
		{
			std::vector<cParticle*>::iterator it2 = ++it; //to not compare to self
			it--; //don't want to edit it without forloop
			for (it2; it2 != particles.end(); it2++) //Avoid duplicates by starting from particle next to the first one
			{
				if ((*it2)->getShape() == Sphere)
				{
					glm::vec3 pos1 = (*it)->GetPosition();
					glm::vec3 pos2 = (*it2)->GetPosition();
					float distance = glm::length(pos1 - pos2);
					bool intersect = distance <= (*it)->getRadius() + (*it2)->getRadius();
					if (intersect)
					{
						contact->mContactNormal = glm::normalize(pos1 - pos2);
						contact->mParticle0 = *it;
						contact->mParticle1 = *it2;
						contact->mPenetration = ((*it)->getRadius() + (*it2)->getRadius()) - distance;
						contact->mRestitution = 0.6f;
						contact++;
						numContactsCreated++;
					}
					if (numContactsCreated >= limit)
					{
						return numContactsCreated;
					}
				}
			}
		}
	}
	return numContactsCreated;
}

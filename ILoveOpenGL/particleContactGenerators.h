#pragma once
#include "glmIncludes.h"
#include "iParticleContactGenerator.h"

//Checks and generates contacts for planes
//One for each plane
class cPlaneContactGenerator : public iParticleContactGenerator
{
	float mOffset;
	glm::vec3 mNormal;
public:
	cPlaneContactGenerator(glm::vec3 normal, float offset);

	/// <summary>
	/// Checks and generates contacts for planes
	/// </summary>
	/// <param name="contact">returned contacts</param>
	/// <param name="limit">amount of contacts allowed to creates</param>
	/// <param name="particles">particles to operate</param>
	/// <returns>amount of contacts created</returns>
	virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
	
};

//checks and generates contacts for spheres
//Unlike plane generator, only one sphereContactGenerator needs to exist
class cSphereContactGenerator : public iParticleContactGenerator
{
public:
	cSphereContactGenerator();

	/// <summary>
	/// Check for intersections, if exists, create contact
	/// Only acknowledges Spheres for contacts
	/// </summary>
	/// <param name="contact">returned contact</param>
	/// <param name="limit">limit of how many contacts can be generated</param>
	/// <param name="particles">Particles to work on</param>
	/// <returns>amount of contacts</returns>
	virtual size_t AddContact(cParticleContact* contact, size_t limit, std::vector<cParticle*>& particles) const;
};
#pragma once

/*
* author:	Bogdan Tsyganok, 0886354
* purpose:	"Point in space", extended to also hold a mesh
*/

#include "glmIncludes.h"
#include "cMesh.h"
#include "iGameObject.h"
#include <vector>

enum eShape { Sphere, Unknown };

class cParticle
{
private:
	// Linear position of the particle in world space
	glm::vec3 positionXYZ;
	// Linear velocity of the particle in world space
	glm::vec3 mVelocity;
	// Linear acceleration of the particle in world space
	glm::vec3 mAcceleration;
	// Inverse mass
	float mInverseMass;
	// Externally applied forces accumulated during a time step
	glm::vec3 mAppliedForce;
	// Damping is applied to linear motion
	// Required to remove energy added through numerical instability of integration
	// Also generally used as an overall "friction" for the particle
	float mDamping;

	float mRadius;

	eShape mShape = Unknown;

	std::string friendlyName;
protected:
	// Particle's lifetime
	float mAge;
	// Particle being live
	bool bIsAlive;

	iGameObjectMediator* mgoMediator = NULL;
public:

	// Mesh on top of particle
	cMesh* mesh;
	cParticle(float mass, const glm::vec3& position, float radius = 0);
	virtual ~cParticle();

	cParticle() = delete;
	cParticle(cParticle& other) = delete;
	cParticle& operator=(cParticle& other) = delete;

	void SetDamping(float damping);
	float GetDamping() const;

	float GetMass() const;
	float GetInverseMass() const;

	void SetShape(eShape shape);
	eShape getShape() const;

	bool IsStaticObject() const;

	glm::vec3 GetPosition() const;
	void GetPosition(glm::vec3& position);
	void SetPosition(const glm::vec3& position);

	glm::vec3 GetVelocity() const;
	void GetVelocity(glm::vec3& velocity);
	void SetVelocity(const glm::vec3& velocity);

	glm::vec3 GetAcceleration() const;
	void GetAcceleration(glm::vec3& acceleration);
	void SetAcceleration(const glm::vec3& acceleration);

	std::string getFriendlyName() const;
	void getFriendlyName(std::string& friendlyName);
	void setFriendlyName(const std::string& friendlyName);

	float getRadius();

	void ApplyForce(const glm::vec3& force);
	void ClearAppliedForces();

	virtual void Integrate(float deltaTime);

	virtual bool IsAlive();
	//Makes children
	virtual void GenerateChildren(std::vector<cParticle* > &particleOut);

	void SetMediator(iGameObjectMediator* mediator);

};
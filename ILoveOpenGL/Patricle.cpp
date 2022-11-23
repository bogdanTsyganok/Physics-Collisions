#include "Particle.h"
#include <vector>

float cParticle::getRadius()
{
	return mRadius;
}

cParticle::cParticle(float mass, const glm::vec3& position, float radius)
	: positionXYZ(position)
	, mVelocity(0.f)
	, mAcceleration(0.f)
	, mDamping(0.995f)
	, mAppliedForce(0.f)
	, bIsAlive(true)
	, mAge(0)
{
	
	mesh = new cMesh();
	mesh->positionXYZ = positionXYZ;
	positionXYZ = position;
	mRadius = radius;
	if (mass <= 0.f)
	{
		mInverseMass = 0.f;
	}
	else
	{
		mInverseMass = 1.f / mass;
	}
}
cParticle::~cParticle()
{
	delete mesh;
}

void cParticle::SetDamping(float damping)
{
	mDamping = glm::clamp(damping, 0.f, 1.f);
}
float cParticle::GetDamping() const
{
	return mDamping;
}

float cParticle::GetMass() const
{
	if (mInverseMass == 0.f)
	{
		return mInverseMass;
	}
	return 1.0f / mInverseMass;
}

float cParticle::GetInverseMass() const
{
	return mInverseMass;
}

void cParticle::SetShape(eShape shape)
{
	mShape = shape;
}

eShape cParticle::getShape() const
{
	return mShape;
}

bool cParticle::IsStaticObject() const
{
	return mInverseMass == 0.f;
}

glm::vec3 cParticle::GetPosition() const
{
	return positionXYZ;
}
void cParticle::GetPosition(glm::vec3& position)
{
	position = positionXYZ;
}
void cParticle::SetPosition(const glm::vec3& position)
{
	positionXYZ = position;
}

glm::vec3 cParticle::GetVelocity() const
{
	return mVelocity;
}
void cParticle::GetVelocity(glm::vec3& velocity)
{
	velocity = mVelocity;
}
void cParticle::SetVelocity(const glm::vec3& velocity)
{
	mVelocity = velocity;
}

glm::vec3 cParticle::GetAcceleration() const
{
	return mAcceleration;
}
void cParticle::GetAcceleration(glm::vec3& acceleration)
{
	acceleration = mAcceleration;
}
void cParticle::SetAcceleration(const glm::vec3& acceleration)
{
	mAcceleration = acceleration;
}

std::string cParticle::getFriendlyName() const
{
	return this->friendlyName;
}

void cParticle::getFriendlyName(std::string& friendlyName)
{
	friendlyName = this->friendlyName;
}

void cParticle::setFriendlyName(const std::string& friendlyName)
{
	this->friendlyName = friendlyName;
}

void cParticle::ApplyForce(const glm::vec3& force)
{
	mAppliedForce += force;
}
void cParticle::ClearAppliedForces()
{
	mAppliedForce = glm::vec3(0.f, 0.f, 0.f);
}

void cParticle::GenerateChildren(std::vector<cParticle* > &particleOut)
{
	//Base level Particle doesn't generate children
}

void cParticle::SetMediator(iGameObjectMediator* mediator)
{
	mgoMediator = mediator;
}

void cParticle::Integrate(float deltaTime)
{
	if (mInverseMass == 0.f)
	{
		return; // static things don't move!
	}

	positionXYZ += mVelocity * deltaTime;
	mesh->positionXYZ += mVelocity * deltaTime;
	mVelocity += (mAcceleration + mAppliedForce * mInverseMass) * deltaTime;
	if(mgoMediator != NULL)
	{
		NVM updateMessage = { "UpdateMeshPosition", glm::vec4(positionXYZ, 1.0f), "" };
		mgoMediator->Notify(Sender::Particle, updateMessage);
	}
	// apply damping
	mVelocity *= glm::pow(mDamping, deltaTime);

	// clear applied forces
	ClearAppliedForces();
}

bool cParticle::IsAlive()
{
	return bIsAlive;
}

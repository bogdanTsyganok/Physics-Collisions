#include "GravityGenerator.h"

cParticleGravityGenerator::cParticleGravityGenerator(const glm::vec3& gravityAcceleration)
	: gravityAcceleration(gravityAcceleration)
{

}

cParticleGravityGenerator::~cParticleGravityGenerator()
{

}

void cParticleGravityGenerator::Update(cParticle* particle, float deltaTime)
{
	// F = M*A
	particle->ApplyForce(gravityAcceleration * particle->GetMass());
}
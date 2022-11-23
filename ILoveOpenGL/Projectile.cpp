#include "Projectile.h"

cProjectile::cProjectile(float mass, glm::vec3 pos) : cParticle(mass, pos)
{
}

cProjectile::~cProjectile()
{
}

bool cProjectile::IsAlive()
{
	return bIsAlive;
}

void cProjectile::Integrate(float deltaTime)
{
	cParticle::Integrate(deltaTime);
	this->mAge += deltaTime;
	if (mAge > _deadAge || GetPosition().y < _deadHeight)
		bIsAlive = false;
}

void cProjectile::Die()
{
	bIsAlive = false;
}

float cProjectile::getDeadHeight()
{
	return _deadHeight;
}

void cProjectile::setDeadHeight(float deadHeight)
{
	_deadHeight = deadHeight;
}

float cProjectile::getDeadAge()
{
	return _deadAge;
}

void cProjectile::setDeadAge(float deadAge)
{
	_deadAge = deadAge;
}

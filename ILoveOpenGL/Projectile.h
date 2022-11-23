#pragma once


/*
* author:	Bogdan Tsyganok, 0886354
* purpose:	customizable bullet class 
*/

#include "Particle.h"
#include <vector>
#include "helpers.h"
/*
* author:		Bogdan Tsyganok, 0886354
* encapsulates any kind of projectile and clauses for it to despawn
*/

#include "ParicleWorld.h"
#include "GravityGenerator.h"

class cProjectile : public cParticle
{
	//The height offset below which particle will die
	float _deadHeight;

	//The age at which particle will die
	float _deadAge;
public:
	cProjectile(float mass, glm::vec3 pos);
	virtual ~cProjectile();
	virtual bool IsAlive();
	virtual void Integrate(float deltaTime);
	virtual void Die();

	float getDeadHeight();
	void setDeadHeight(float deadHeight);

	float getDeadAge();
	void setDeadAge(float deadAge);
};
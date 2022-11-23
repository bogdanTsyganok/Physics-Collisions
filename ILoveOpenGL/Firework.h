#pragma once

/*
* author:	Bogdan Tsyganok, 0886354
* purpose:	base level firework + additional versions with patterns
*/

#include "Particle.h"
#include <vector>
#include "helpers.h"
#include "ParicleWorld.h"
#include "GravityGenerator.h"

class cFirework : public cParticle
{
protected:
	cParticleWorld* _world;
public:
	cFirework(float mass, glm::vec3 pos, cParticleWorld* world);
	virtual ~cFirework();
	virtual bool IsAlive();
	virtual void Integrate(float deltaTime);
};

class cBigFirework : public cFirework
{
public:
	cBigFirework(float mass, glm::vec3 pos, cParticleWorld* world);
	virtual ~cBigFirework();
	virtual bool IsAlive();
	virtual void Integrate(float deltaTime);

	virtual void GenerateChildren(std::vector<cParticle* > &particleOut);

	static cBigFirework* Create(glm::vec3 pos, glm::vec3 initForce, cParticleWorld* world);

};

class cBloomFirework : public cFirework
{
public:
	cBloomFirework(float mass, glm::vec3 pos, cParticleWorld* world);
	virtual ~cBloomFirework();
	virtual bool IsAlive();
	virtual void Integrate(float deltaTime);

	virtual void GenerateChildren(std::vector<cParticle* > &particleOut);

	static cBloomFirework* Create(glm::vec3 pos, glm::vec3 initForce, cParticleWorld* world);

};

class cJiggleFirework : public cFirework
{

public:
	cJiggleFirework(float mass, glm::vec3 pos, cParticleWorld* world);
	virtual ~cJiggleFirework();
	virtual bool IsAlive();
	virtual void Integrate(float deltaTime);

	virtual void GenerateChildren(std::vector<cParticle* >& particleOut);

	static cJiggleFirework* Create(glm::vec3 pos, glm::vec3 initForce, cParticleWorld* world);

};

class cReverseJiggleFirework : public cFirework
{

public:
	cReverseJiggleFirework(float mass, glm::vec3 pos, cParticleWorld* world);
	virtual ~cReverseJiggleFirework();
	virtual bool IsAlive();
	virtual void Integrate(float deltaTime);

	virtual void GenerateChildren(std::vector<cParticle* >& particleOut);

	static cReverseJiggleFirework* Create(glm::vec3 pos, glm::vec3 initForce, cParticleWorld* world);

};
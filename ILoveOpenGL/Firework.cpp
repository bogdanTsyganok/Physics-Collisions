#include "Firework.h"
#include <iostream>

cFirework::~cFirework()
{

}

cFirework::cFirework(float mass, glm::vec3 pos, cParticleWorld* world):cParticle(mass, pos)
{
	this->_world = world;
	bIsAlive = true;
	mAge = 0;
}

bool cFirework::IsAlive()
{
	return bIsAlive;
}

void cFirework::Integrate(float deltaTime)
{
	cParticle::Integrate(deltaTime);
	this->mAge += deltaTime;
	if (mAge > 2.0f)
		bIsAlive = false;
}


cBigFirework::cBigFirework(float mass, glm::vec3 pos, cParticleWorld* world) : cFirework(mass, pos, world)
{
}

cBigFirework::~cBigFirework()
{

}

bool cBigFirework::IsAlive()
{
	return bIsAlive;
}

void cBigFirework::Integrate(float deltaTime)
{
	cParticle::Integrate(deltaTime);
	if (this->GetVelocity().length() < 4.0f)
	{
		bIsAlive = false;
	}
}

void cBigFirework::GenerateChildren(std::vector<cParticle* > &particleOut)
{
	for (int i = 0; i < 15; i++)
	{
		cFirework* spark = new cFirework(0.002f, this->GetPosition(), this->_world);
		spark->ApplyForce(getRandomVector(10));
		spark->mesh->meshName = "assets/bun_zipper_res2_xyz_n_rgb.ply";
		spark->mesh->scale = 5.0f;
		particleOut.push_back(spark);
	}
}

cBigFirework* cBigFirework::Create(glm::vec3 pos, glm::vec3 initForce, cParticleWorld* world)
{
	cBigFirework* firework = new cBigFirework(0.04f, pos, world);
	firework->ApplyForce(initForce);
	firework->SetDamping(0.45f);
	return firework;
}

cBloomFirework::cBloomFirework(float mass, glm::vec3 pos, cParticleWorld* world) : cFirework(mass, pos, world)
{
}

cBloomFirework::~cBloomFirework()
{
}

bool cBloomFirework::IsAlive()
{
	return this->bIsAlive;
}

void cBloomFirework::Integrate(float deltaTime)
{
	cParticle::Integrate(deltaTime);
	if (this->GetVelocity().y < 4.0f)
	{
		bIsAlive = false;
	}
}

void cBloomFirework::GenerateChildren(std::vector<cParticle*>& particleOut)
{
	for (int i = 0; i < 15; i++)
	{
		cFirework* spark = new cFirework(0.002f, this->GetPosition(), this->_world);
		spark->SetVelocity(getRandomCircularVector(5.0f, glm::two_over_pi<float>()));
		spark->SetAcceleration(glm::vec3(0.0f, -4.8f, 0.0f));
		spark->mesh->meshName = "assets/bun_zipper_res2_xyz_n_rgb.ply";
		spark->mesh->scale = 5.0f;
		spark->mesh->bUseObjectDebugColour = true;
		spark->mesh->objectDebugColourRGBA = glm::vec4(1, 0, 0, 1);
		particleOut.push_back(spark);
	}
}

cBloomFirework* cBloomFirework::Create(glm::vec3 pos, glm::vec3 initForce, cParticleWorld* world)
{
	cBloomFirework* firework = new cBloomFirework(0.04f, pos, world);
	firework->mesh->bUseObjectDebugColour = true;
	firework->mesh->objectDebugColourRGBA = glm::vec4(155.0f, 50.0f, 0.f, 1.0f);
	firework->ApplyForce(initForce);
	firework->SetDamping(0.6f);
	return firework;
}

cJiggleFirework::cJiggleFirework(float mass, glm::vec3 pos, cParticleWorld* world) : cFirework(mass, pos, world)
{
}

cJiggleFirework::~cJiggleFirework()
{
}

bool cJiggleFirework::IsAlive()
{
	return bIsAlive;
}

void cJiggleFirework::Integrate(float deltaTime)
{
	cParticle::Integrate(deltaTime);
	glm::vec3 jiggleForce = glm::vec3(getRandom(-1, 1), 0.0f, getRandom(-1, 1));
	this->ApplyForce(jiggleForce);
	if (this->GetVelocity().y < 3.0f)
	{
		bIsAlive = false;
	}
}

void cJiggleFirework::GenerateChildren(std::vector<cParticle*>& particleOut)
{
	for (int i = 0; i < 15; i++)
	{
		cReverseJiggleFirework* spark = new cReverseJiggleFirework(0.002f, this->GetPosition(), this->_world);
		spark->SetVelocity(getRandomCircularVector(8.0f, glm::pi<float>() - glm::quarter_pi<float>()));
		spark->SetAcceleration(glm::vec3(0.0f, -4.8f, 0.0f));
		spark->mesh->meshName = "assets/bun_zipper_res2_xyz_n_rgb.ply";
		spark->mesh->scale = 5.0f;
		spark->mesh->bUseObjectDebugColour = true;
		spark->mesh->objectDebugColourRGBA = glm::vec4(0, 0, 1, 1);
		particleOut.push_back(spark);
	}
}

cJiggleFirework* cJiggleFirework::Create(glm::vec3 pos, glm::vec3 initForce, cParticleWorld* world)
{
	cJiggleFirework* firework = new cJiggleFirework(0.04f, pos, world);
	firework->mesh->bUseObjectDebugColour = true;
	firework->mesh->objectDebugColourRGBA = glm::vec4(15.0f, 50.0f, 240.f, 1.0f);
	firework->ApplyForce(initForce);
	firework->SetDamping(0.6f);
	return firework;
}

cReverseJiggleFirework::cReverseJiggleFirework(float mass, glm::vec3 pos, cParticleWorld* world) : cFirework(mass, pos, world)
{
}

cReverseJiggleFirework::~cReverseJiggleFirework()
{
}

bool cReverseJiggleFirework::IsAlive()
{
	return bIsAlive;
}

void cReverseJiggleFirework::Integrate(float deltaTime)
{
	cParticle::Integrate(deltaTime);
	glm::vec3 jiggleForce = glm::vec3(getRandom(-1, 1), 0.0f, getRandom(-1, 1));
	this->ApplyForce(jiggleForce);
	mAge += deltaTime;
	if (mAge > 1.0f)
	{
		bIsAlive = false;
	}
}

void cReverseJiggleFirework::GenerateChildren(std::vector<cParticle*>& particleOut)
{
	for (int i = 0; i < 15; i++)
	{
		cFirework* spark = new cFirework(0.002f, this->GetPosition(), this->_world);
		spark->SetVelocity(getRandomVector(8.0f));
		spark->SetAcceleration(glm::vec3(0.0f, -4.8f, 0.0f));
		spark->mesh->meshName = "assets/bun_zipper_res2_xyz_n_rgb.ply";
		spark->mesh->scale = 5.0f;
		spark->mesh->bUseObjectDebugColour = true;
		spark->mesh->objectDebugColourRGBA = glm::vec4(1, 0, 0, 1);
		particleOut.push_back(spark);
	}
}

cReverseJiggleFirework* cReverseJiggleFirework::Create(glm::vec3 pos, glm::vec3 initForce, cParticleWorld* world)
{
	cReverseJiggleFirework* firework = new cReverseJiggleFirework(0.04f, pos, world);
	firework->mesh->bUseObjectDebugColour = true;
	firework->mesh->objectDebugColourRGBA = glm::vec4(15.0f, 50.0f, 240.f, 1.0f);
	firework->ApplyForce(initForce);
	firework->SetDamping(0.6f);
	return firework;
}

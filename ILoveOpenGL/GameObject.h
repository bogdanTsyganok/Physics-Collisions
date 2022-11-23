#pragma once
#include "cMesh.h"
#include "Particle.h"
#include "iGameObject.h"

class cGameOjbect : public iGameObjectMediator
{
private:
	cMesh* mMesh;
	cParticle* mParticle;
public:
	//Constructors and destructors
	cGameOjbect();
	cGameOjbect(cMesh* newMesh, cParticle* newParticle);
	~cGameOjbect();

	//Getters and setters
	cMesh* GetMesh();
	cParticle* GetParticle();


	bool IsAlive();
	void SetMesh(cMesh* newMesh);
	void SetParticle(cParticle* newParticle);

	void Render();

	virtual bool Notify(Sender sender, NVM data);
};
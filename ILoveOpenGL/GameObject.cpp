#include "GameObject.h"

cGameOjbect::cGameOjbect()
{
	this->mMesh = NULL;
	this->mParticle = NULL;
}
cGameOjbect::cGameOjbect(cMesh* newMesh, cParticle* newParticle)
{
	this->mMesh = newMesh;
	if (newMesh != NULL)
	{
		this->mMesh->SetMediator(this);
	}
	this->mParticle = newParticle;
	if (newParticle != NULL)
	{
		this->mParticle->SetMediator(this);
	}
}
cGameOjbect::~cGameOjbect()
{
	if (mMesh != NULL)
		delete mMesh;
	if (mParticle != NULL)
		delete mParticle;
}
cMesh* cGameOjbect::GetMesh()
{
	return mMesh;
}

cParticle* cGameOjbect::GetParticle()
{
	return mParticle;
}

bool cGameOjbect::IsAlive()
{
	return mParticle->IsAlive();
}

void cGameOjbect::SetMesh(cMesh* newMesh)
{
	newMesh->SetMediator(this);
	mMesh = newMesh;
}

void cGameOjbect::SetParticle(cParticle* newParticle)
{
	//newParticle->SetMediator(this);
	mParticle = newParticle;
}

void cGameOjbect::Render()
{
	this->GetMesh()->positionXYZ = this->GetParticle()->GetPosition();
}

bool cGameOjbect::Notify(Sender sender, NVM data)
{
	if (sender == Sender::Mesh)//sender type is mesh
	{
		//Shouldn't happen yet
		//this->mParticle->SetPosition(this->mMesh->positionXYZ);
	}
	else if (sender == Sender::Particle)//sender type is particle
	{
		if (data.command == "UpdateMeshPosition")
		{
			glm::vec3 particlePosition = data.floatData;
			this->mMesh->positionXYZ = particlePosition;
		}
		else if (data.command == "EnchanceSize")
		{
			this->mMesh->scale *= data.floatData.x;
		}
		else
		{
			//something is wrong
		}
	}
	return true;
}

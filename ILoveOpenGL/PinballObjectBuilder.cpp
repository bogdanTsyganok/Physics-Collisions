#include "PinballObjectBuilder.h"
#include "helpers.h"
#include "ParticleConstraints.h"

void cPinballObjectBuilder::recolourAtDistance(int index)
{
    if (index < sphereCollections.size())
    {
        for (cGameOjbect* m : sphereCollections[index])
        {
            m->GetMesh()->objectDebugColourRGBA = glm::vec4((getRandom() * 100 + 150) / 255.f, (getRandom() * 25 + 100) / 255.f, (getRandom() * 50 + 150) / 255.f, 1.f);;
        }
    }
}

void cPinballObjectBuilder::createAtOffset(glm::vec3 offset, cPinballToy blueprint, std::vector<cGameOjbect*>& meshes, cParticleWorld* particleWorld, std::vector<cParticleRodConstraint*>& constraintsOut, iParticleForceGenerator* gravity)
{
	std::vector<glm::vec4> spheres = blueprint.getObjects();
	std::vector<std::pair<int, int>> constraints = blueprint.getConstraints();
    std::vector<cParticle*> createdParticles;
    std::vector<cGameOjbect*> objectSpheres;
	for (int i = 0; i < spheres.size(); i++)
	{
        cMesh* testMeshBall1 = new cMesh();
        testMeshBall1->meshName = "assets/Sphere_xyz_n_rgba.ply";
        testMeshBall1->bUseObjectDebugColour = true;
        testMeshBall1->objectDebugColourRGBA = glm::vec4((getRandom() * 100 + 150) / 255.f, (getRandom() * 25 + 100) / 255.f, (getRandom() * 50 + 150) / 255.f, 1.f);
        testMeshBall1->positionXYZ = glm::vec3(spheres[i].x, spheres[i].y, spheres[i].z) + offset;

        testMeshBall1->scale *= spheres[i].a;
        cParticle* testParticleBall1 = new cParticle(0.5f, testMeshBall1->positionXYZ, 1.0f * spheres[i].a);
        testParticleBall1->SetShape(Sphere);

        cGameOjbect* testBall1 = new cGameOjbect(testMeshBall1, testParticleBall1);
        meshes.push_back(testBall1);
        particleWorld->AddParticle(testParticleBall1);
        createdParticles.push_back(testParticleBall1);
        objectSpheres.push_back(testBall1);

        if (gravity != NULL)
        {
            particleWorld->GetForceRegistry()->Register(testParticleBall1, gravity);
        }
	}

    sphereCollections.push_back(objectSpheres);

    for (int i = 0; i < constraints.size(); i++)
    {
        cParticleRodConstraint* constraint = new cParticleRodConstraint(createdParticles[constraints[i].first], createdParticles[constraints[i].second]);
        particleWorld->AddContactGenerator(constraint);
        constraintsOut.push_back(constraint);
    }
}

void cPinballObjectBuilder::applyForceToToy(glm::vec3 force, int index)
{
    if (index < sphereCollections.size() && index >= 0)
    {
        for (cGameOjbect* object : sphereCollections[index])
        {
            object->GetParticle()->SetVelocity(force);
        }
    }
}

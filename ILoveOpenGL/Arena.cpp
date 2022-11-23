#include "Arena.h"
#include "helpers.h"

cArena::cArena(float offset, float angle)
{
    float cosAngle = glm::cos(glm::radians(angle));
    float sinAngle = glm::sin(glm::radians(angle));
    float offsetSin = sinAngle * offset;
    float offsetCos = cosAngle * offset;
    {
        cMesh* wallMeshW = new cMesh();
        wallMeshW->meshName = "assets/ground.ply";
        wallMeshW->scale = 0.75f;
        wallMeshW->positionXYZ = glm::vec3(-offsetCos, -offsetSin, 0.f);
        wallMeshW->orientationXYZ = glm::vec3(0, 0, glm::radians(90.f + angle));
        wallMeshW->bUseObjectDebugColour = true;
        wallMeshW->objectDebugColourRGBA = glm::vec4(0.6f, 0.6f, 0.6f, 0.f);
        mContactGenerators.push_back(new cPlaneContactGenerator(glm::vec3(cosAngle, sinAngle, 0.f), -offset));
        cGameOjbect* wallWObject = new cGameOjbect(wallMeshW, NULL);
        mObjects.push_back(wallWObject);
    }
    {
        cMesh* wallMeshE = new cMesh();
        wallMeshE->meshName = "assets/ground.ply";
        wallMeshE->scale = 0.75f;
        wallMeshE->positionXYZ = glm::vec3(offsetCos, -offsetSin, 0.f);
        wallMeshE->orientationXYZ = glm::vec3(0, 0, glm::radians(90.f - angle));
        wallMeshE->bUseObjectDebugColour = true;
        wallMeshE->objectDebugColourRGBA = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);
        mContactGenerators.push_back(new cPlaneContactGenerator(glm::vec3(-cosAngle, sinAngle, 0.f), -offset));
        cGameOjbect* wallEObject = new cGameOjbect(wallMeshE, NULL);
        mObjects.push_back(wallEObject);
    }
    {
        cMesh* wallMeshS = new cMesh();
        wallMeshS->meshName = "assets/ground.ply";
        wallMeshS->scale = 0.75f;
        wallMeshS->positionXYZ = glm::vec3(0.f, -offsetSin, -offsetCos);
        wallMeshS->orientationXYZ = glm::vec3(glm::radians(90.f - angle), 0, 0);
        wallMeshS->bUseObjectDebugColour = true;
        wallMeshS->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 0.f);
        mContactGenerators.push_back(new cPlaneContactGenerator(glm::vec3(0.f, sinAngle, cosAngle), -offset));
        cGameOjbect* wallSObject = new cGameOjbect(wallMeshS, NULL);
        mObjects.push_back(wallSObject);
    }
    {
        cMesh* wallMeshS = new cMesh();
        wallMeshS->meshName = "assets/ground.ply";
        wallMeshS->scale = 0.75f;
        wallMeshS->positionXYZ = glm::vec3(0.f, -offsetSin, offsetCos);
        wallMeshS->orientationXYZ = glm::vec3(glm::radians(90.f + angle), 0, 0);
        wallMeshS->bUseObjectDebugColour = true;
        wallMeshS->objectDebugColourRGBA = glm::vec4(0.8f, 0.8f, 0.8f, 0.f);
        mContactGenerators.push_back(new cPlaneContactGenerator(glm::vec3(0.f, sinAngle, -cosAngle), -offset));
        cGameOjbect* wallSObject = new cGameOjbect(wallMeshS, NULL);
        mObjects.push_back(wallSObject);
    }
}

void cArena::addSphere(glm::vec3 pos, float radius)
{

    cMesh* testMeshBall1 = new cMesh();
    testMeshBall1->meshName = "assets/Sphere_xyz_n_rgba.ply";
    testMeshBall1->bUseObjectDebugColour = true;
    testMeshBall1->objectDebugColourRGBA = glm::vec4((getRandom() * 100 + 150) / 255.f, (getRandom() * 25 + 100) / 255.f, (getRandom() * 50 + 150) / 255.f, 1.f);
    testMeshBall1->positionXYZ = pos;

    testMeshBall1->scale = radius;
    cParticle* testParticleBall1 = new cParticle(0.f, testMeshBall1->positionXYZ, radius);
    testParticleBall1->SetShape(Sphere);

    cGameOjbect* testBall1 = new cGameOjbect(testMeshBall1, testParticleBall1);
    mObjects.push_back(testBall1);
}

std::vector<iParticleContactGenerator*> cArena::getGenerators()
{
    return mContactGenerators;
}

std::vector<cGameOjbect*> cArena::getObjects()
{
    return mObjects;
}

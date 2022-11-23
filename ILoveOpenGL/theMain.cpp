

#include "GLCommon.h"


#include "glmIncludes.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>       // "smart array"
#include <fstream>      // C++ file I-O library (look like any other stream)
#include <limits>

#include "cVAOManager.h"
#include "cMesh.h"
#include "cShaderManager.h"

#include "cLightManager.h"

#include "Particle.h"
#include "ParicleWorld.h"
#include "Projectile.h"
#include "GravityGenerator.h"
#include "particleContactGenerators.h"
#include "ParticleConstraints.h"
#include "Arena.h"

#include "ObjectLoader.h"
#include "CannonLoader.h"
#include "GameObject.h"
#include "PinballObjectBuilder.h"


#include "helpers.h"

// Global things are here:
glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
glm::vec3 cameraTarget = glm::vec3(0, 0, 0);

cVAOManager  gVAOManager;
cShaderManager  gShaderManager;
bool gWireframeMode = false;
bool gDebugSphereEnabled = false;
float previousTime = static_cast<float>(glfwGetTime());
glm::mat3x3 gOrthonormalBasis;
float deltaTime;

cGameOjbect* cannon = nullptr;
sCannonConfig cannonConfiguration;

cLightManager gTheLights;
size_t gSelectedModel = 0;
bool renderConstraints = false;

cParticleWorld* g_world = nullptr;
cParticleGravityGenerator* g_gravityGenerator = nullptr;
std::vector<cParticleRodConstraint*> g_constraints;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void LoadCameraPos()
{
    cameraEye.x = -58;
    cameraEye.y = 34;
    cameraEye.z = -51;
    cameraTarget.x = 0;
    cameraTarget.y = 4;
    cameraTarget.z = -4;
}

std::vector<cGameOjbect*> gameObjects;

/// <summary>
/// Shoot ball
/// </summary>
void CreateProjectile(std::string objectName);



objectMap objects;
cPinballObjectBuilder builder;
int selectedToy = 0;

std::vector<glm::vec3> createPositions;


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    float cameraSpeed = 1.0f;
    
    // Basic camera controls
    if (key == GLFW_KEY_A)
    {
        cameraEye.x -= cameraSpeed;     // Go left
        std::cout << cameraEye.x << " " << cameraEye.y << " " << cameraEye.z << std::endl;
    }
    if (key == GLFW_KEY_D)
    {
        cameraEye.x += cameraSpeed;     // Go right
    }

    if (key == GLFW_KEY_W)
    {
        cameraEye.z += cameraSpeed;     // Go forward
    }
    if (key == GLFW_KEY_S)
    {
        cameraEye.z -= cameraSpeed;     // Go backwards
    }

    if (key == GLFW_KEY_Q)
    {
        cameraEye.y -= cameraSpeed;     // Go "Down"
    }
    if (key == GLFW_KEY_E)
    {
        cameraEye.y += cameraSpeed;     // Go "Up"
    }
    
    //if (key == GLFW_KEY_J)
    //{
    //    cameraTarget.x -= cameraSpeed;     // Go left
    //}
    //if (key == GLFW_KEY_L)
    //{
    //    cameraTarget.x += cameraSpeed;     // Go right
    //}

    //if (key == GLFW_KEY_I)
    //{
    //    cameraTarget.z += cameraSpeed;     // Go forward
    //}
    //if (key == GLFW_KEY_K)
    //{
    //    cameraTarget.z -= cameraSpeed;     // Go backwards
    //}

    if (key == GLFW_KEY_O)
    {
        cameraTarget.y -= cameraSpeed;     // Go "Down"
    }
    if (key == GLFW_KEY_U)
    {
        cameraTarget.y += cameraSpeed;     // Go "Up"
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        renderConstraints = !renderConstraints;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        if (selectedToy > 0)
        {
            builder.recolourAtDistance(selectedToy--);
        }
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        if (selectedToy < (builder.sphereCollections.size() - 1))
        {
            builder.recolourAtDistance(selectedToy++);
        }
    }

    if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS)
    {
        builder.applyForceToToy(glm::vec3(0.f, -10.f, 0.f),selectedToy);
    }

    if (key == GLFW_KEY_KP_8 && action == GLFW_PRESS)
    {
        builder.applyForceToToy(glm::vec3(0.f, 10.f, 0.f),selectedToy);
    }

    if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS)
    {
        glm::vec3 vectorLeft = -glm::cross(glm::normalize(::cameraTarget - ::cameraEye), glm::vec3(0.f, 1.f, 0.f));
        builder.applyForceToToy(glm::normalize(vectorLeft) * 25.f,selectedToy);
    }

    if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS)
    {
        glm::vec3 vectorRight = glm::cross(glm::normalize(::cameraTarget - ::cameraEye), glm::vec3(0.f, 1.f, 0.f));
        builder.applyForceToToy(glm::normalize(vectorRight) * 25.f, selectedToy);
    }

    if (key == GLFW_KEY_KP_7 && action == GLFW_PRESS)
    {
        glm::vec3 vectorForward = glm::cross(glm::cross(glm::normalize(::cameraTarget - ::cameraEye), glm::vec3(0.f, 1.f, 0.f)), glm::vec3(0.f, 1.f, 0.f));
        builder.applyForceToToy(glm::normalize(vectorForward) * 25.f, selectedToy);
    }

    if (key == GLFW_KEY_KP_9 && action == GLFW_PRESS)
    {
        glm::vec3 vectorBack = -glm::cross(glm::cross(glm::normalize(::cameraTarget - ::cameraEye), glm::vec3(0.f, 1.f, 0.f)), glm::vec3(0.f, 1.f, 0.f));
        builder.applyForceToToy(glm::normalize(vectorBack) * 25.f, selectedToy);
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    { 
        CreateProjectile("LinePiece");
    }

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    { 
        CreateProjectile("Cube");
    }

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    { 
        CreateProjectile("TetraChedron");
    }

    //Render everything in wireframe mode
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        gDebugSphereEnabled = !gDebugSphereEnabled;
    }

}

int main(void)
{
    GLFWwindow* window;
    GLuint program = 0;     // 0 means "no shader program"

    GLint mvp_location = -1;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return -1;
        //exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(1200, 640, "1 Save, 2 Load, 3-4 switch selected model, 5 toggle wireframe, 6 toggle lookat sphere", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    // Tiny change from the original documentation code
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    GLint max_uniform_location = 0;
    GLint* p_max_uniform_location = NULL;
    p_max_uniform_location = &max_uniform_location;
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);

    std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max_uniform_location << std::endl;


    cObjectsLoader::readProjectileConfig("configs/ObjectConfig.json", objects);

    cShaderManager::cShader vertShader;
    vertShader.fileName = "assets/shaders/vertShader_01.glsl";

    cShaderManager::cShader fragShader;
    fragShader.fileName = "assets/shaders/fragShader_01.glsl";

    if (gShaderManager.createProgramFromFile("Shader#1", vertShader, fragShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        program = gShaderManager.getIDFromFriendlyName("Shader#1");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << gShaderManager.getLastError() << std::endl;
    }


    //GLint mvp_location = -1;
    mvp_location = glGetUniformLocation(program, "MVP");

    // Get "uniform locations" (aka the registers these are in)
    GLint matModel_Location = glGetUniformLocation(program, "matModel");
    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
    GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");

    gOrthonormalBasis = orthonormalBasis(glm::vec3(1, 0, 0), glm::vec3(0, 0, 1));


    gTheLights.theLights[0].param1.x = 2.f;
    gTheLights.theLights[0].param2.x = 1.f;
    gTheLights.theLights[0].direction.y = -1.f;

    sModelDrawInfo modelBunny;
    //    if (gVAOManager->LoadModelIntoVAO("bun_zipper_res2 (justXYZ).ply", modelBunny, program))
    if (gVAOManager.LoadModelIntoVAO("assets/bun_zipper_res2_xyz_n_rgb.ply", modelBunny, program))
    {
        std::cout << "Loaded the model: " << modelBunny.meshName << std::endl;
        std::cout << modelBunny.numberOfVertices << " vertices loaded" << std::endl;
        std::cout << modelBunny.numberOfTriangles << " triangles loaded" << std::endl;
    }
    else
    {
        std::cout << "Error: Didn't load the model OK" << std::endl;
    }

    sModelDrawInfo sphereModel;
    if (!gVAOManager.LoadModelIntoVAO("assets/Sphere_xyz_n_rgba.ply", sphereModel, program))
    {
        std::cout << "Didn't load sphere." << std::endl;
    }

    sModelDrawInfo groundModel;
    if (!gVAOManager.LoadModelIntoVAO("assets/ground.ply", groundModel, program))
    {
        std::cout << "Didn't load ground." << std::endl;
    }

    sModelDrawInfo cannonModel;
    if (!gVAOManager.LoadModelIntoVAO("assets/CANNON.ply", cannonModel, program))
    {
        std::cout << "Didn't load CANNON." << std::endl;
    }
    

    sModelDrawInfo rodModel;
    if (!gVAOManager.LoadModelIntoVAO("assets/rod.ply", rodModel, program))
    {
        std::cout << "Didn't load CANNON." << std::endl;
    }

    sModelDrawInfo rodModelY;
    gVAOManager.LoadModelIntoVAO("assets/rodY.ply", rodModelY, program);

    sModelDrawInfo rodModelZ;
    gVAOManager.LoadModelIntoVAO("assets/rodZ.ply", rodModelZ, program);


    std::cout << "Done loading models" << std::endl;

    // Add the models I'd like to draw in the scene   

    if (g_world == nullptr)
        g_world = new cParticleWorld(256, 0);


    g_gravityGenerator = new cParticleGravityGenerator(-9.2f * gOrthonormalBasis[1]);

    //builder.createAtOffset(glm::vec3(0.f, 10.f, 0.f), objects["LinePiece"], gameObjects, g_world, g_constraints, g_gravityGenerator);

    //cParticle* ground = new cParticle(0, glm::vec3(0, 0, 0));
    //cMesh* groundMesh = new cMesh();
    //groundMesh->meshName = "assets/ground.ply";
    //groundMesh->scale = 0.75f;
    //groundMesh->positionXYZ = glm::vec3(0, 0, 0);
    //g_world->AddContactGenerator(new cPlaneContactGenerator(glm::vec3(0.f, 1.f, 0.f), 1.f));
    //g_world->AddParticle(ground);
    //cGameOjbect* groundObject = new cGameOjbect(groundMesh, ground);
    //gameObjects.push_back(groundObject);

    sArenaConfig arenaConfig;
    cCannonLoader::readArenaConfig("configs/ArenaConfig.json", arenaConfig);
    cArena area(arenaConfig.offset, arenaConfig.pitch);
    createPositions.push_back(glm::vec3(0.f, 15.f, 0.f));
    createPositions.push_back(glm::vec3(arenaConfig.offset * 0.5f, 5.f, arenaConfig.offset * 0.7f));
    createPositions.push_back(glm::vec3(arenaConfig.offset * 0.5f, 5.f, arenaConfig.offset * 0.7f));
    createPositions.push_back(glm::vec3(arenaConfig.offset * -0.2f, 5.f, arenaConfig.offset * 0.3f));
    createPositions.push_back(glm::vec3(arenaConfig.offset * -0.8f, 5.f, arenaConfig.offset * -0.8f));

    for (sSphere sphereConfig : arenaConfig.spheres) 
    {
        area.addSphere(glm::vec3(sphereConfig.x, sphereConfig.y, sphereConfig.z), sphereConfig.r);
    }

   /* area.addSphere(glm::vec3(0.f, 5.f, 0.f), 0.5f);
    area.addSphere(glm::vec3(5.f, -4.f, 0.f), 0.5f);
    area.addSphere(glm::vec3(-14.f, 5.f, 5.f), 0.5f);
    area.addSphere(glm::vec3(3.f, 8.f, 12.f), 0.5f);
    area.addSphere(glm::vec3(0.f, 18.f, 0.f), 0.5f);*/

    //area.addSphere(glm::vec3(0.f, 10.f, 0.f), 0.1f);

    for (int i = 0; i < area.getObjects().size(); i++)
    {
        cGameOjbect* go =  area.getObjects()[i];
        if (go->GetParticle() != NULL)
        {
            g_world->AddParticle(go->GetParticle());
        }
        gameObjects.push_back(go);

    }

    for (int i = 0; i < area.getGenerators().size(); i++)
    {
        g_world->AddContactGenerator(area.getGenerators()[i]);
        //}
    }
        //cMesh* cannonMesh = new cMesh();
        //cannonMesh->meshName = "assets/CANNON.ply";
        //cannonMesh->scale = 0.01f;
        //cannonMesh->positionXYZ = glm::vec3(0, 18, 0);
        ////cannon->mesh->orientationXYZ = glm::vec3(glm::radians(45.0f), glm::radians(45.0f), 0);
        ////cannon->setFriendlyName("Cannon");
        //cannon = new cGameOjbect(cannonMesh, NULL);
        //gameObjects.push_back(cannon);
        //


        g_world->AddContactGenerator(new cSphereContactGenerator());


        cCannonLoader::readCannonConfig("configs/CannonConfig.json", cannonConfiguration);
        // cProjectileLoader::readProjectileConfig("configs/ProjectileConfig.json", projectileConfigMap);

        //cannonMesh->orientationXYZ.x = cannonConfiguration.initPitch * glm::pi<float>();
        //cannonMesh->orientationXYZ.y = cannonConfiguration.initYaw * glm::pi<float>();
        ////g_world->AddParticle(cannon);



        LoadCameraPos();

        while (!glfwWindowShouldClose(window))
        {
            float ratio;
            int width, height;
            glm::mat4 matModel;    // used to be "m"; Sometimes it's called "world"
            glm::mat4 p;
            glm::mat4 v;
            glm::mat4 mvp;

            //        mat4x4 m, p, mvp;

            glfwGetFramebufferSize(window, &width, &height);
            ratio = width / (float)height;

            // Turn on the depth buffer
            glEnable(GL_DEPTH);         // Turns on the depth buffer
            glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

            glViewport(0, 0, width, height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

            // *******************************************************
            // Screen is cleared and we are ready to draw the scene...
            // *******************************************************


            // Copy the light information into the shader to draw the scene
            gTheLights.CopyLightInfoToShader();


            float currentTime = static_cast<float>(glfwGetTime());
            deltaTime = currentTime - previousTime;
            previousTime = currentTime;

            if (selectedToy < builder.sphereCollections.size())
            {
                for (cGameOjbect* m : builder.sphereCollections[selectedToy])
                {
                    m->GetMesh()->objectDebugColourRGBA = glm::vec4(.9f, .9f, 0.f, 1.f);
                }
            }
            if(renderConstraints)
            for (int i = 0; i < g_constraints.size(); i++)
            {
                cParticleRodConstraint* constr = g_constraints[i];
                cMesh constrMesh;
                constrMesh.positionXYZ = (constr->GetParticleA()->GetPosition() + constr->GetParticleB()->GetPosition()) / 2.f;
                constrMesh.meshName = "assets/rod.ply";
                constrMesh.scale = 0.01f;
                constrMesh.bUseObjectDebugColour = true;
                constrMesh.objectDebugColourRGBA = glm::vec4(0.4f, 0.4f, 0.4f, 1.f);
                glm::vec3 from = constr->GetParticleA()->GetPosition();
                glm::vec3 to = (constr->GetParticleB()->GetPosition());

                glm::vec3 crossthing = cross(to, from);
                float angle = acos(dot(to, from) / (length(to) * length(from)));
                glm::mat4 matRotation = rotate(glm::mat4(1.f), -angle, normalize(crossthing));

                glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                    glm::vec3(constrMesh.scale,  // Scale in X
                        constrMesh.scale,  // Scale in Y
                        constrMesh.scale));// Scale in Z

                glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                    constrMesh.positionXYZ);

                glm::mat4 matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
                matModel = matModel * matTranslate;

                matModel = matModel * matRotation;

                matModel = matModel * matScale;
                int width, height;
                glfwGetFramebufferSize(window, &width, &height);
                float ratio = width / (float)height;

                glm::mat4 p = glm::perspective(0.6f,
                    ratio,
                    0.1f,
                    1000.0f);

                glm::mat4 v = glm::mat4(1.0f);

                //glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
                glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

                v = glm::lookAt(cameraEye,     // "eye"
                    cameraTarget,  // "at"
                    upVector);
                //        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);

                //        mvp = p * v * matModel;
                //        mat4x4_mul(mvp, p, m);

                glUseProgram(program);

                //          glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
                    //        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);

                            // Don't need this anymore as it's being done inside the shader:
                            // 
                            // mvp = p * v * matModel;
                            //
                glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));
                glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(v));
                glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(p));

                // Inverse transpose of the model matrix
                // (Used to calculate the normal location in vertex space, using only rotation)
                glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
                glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));



                // Michael won't push this until later...
                GLint bUseDebugColour_Location = glGetUniformLocation(program, "bUseDebugColour");
                GLint objectDebugColour_Location = glGetUniformLocation(program, "objectDebugColour");
                GLint bDontLightObject_Location = glGetUniformLocation(program, "bDontLightObject");


                // See if mesh is wanting the vertex colour override (HACK) to be used?
                if (constrMesh.bUseObjectDebugColour)
                {
                    // Override the colour...
                    glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
                    glUniform4f(objectDebugColour_Location,
                        constrMesh.objectDebugColourRGBA.r,
                        constrMesh.objectDebugColourRGBA.g,
                        constrMesh.objectDebugColourRGBA.b,
                        constrMesh.objectDebugColourRGBA.a);
                }
                else
                {
                    // DON'T override the colour
                    glUniform1f(bUseDebugColour_Location, (float)GL_FALSE);
                }


                // See if mesh is wanting the vertex colour override (HACK) to be used?
                if (constrMesh.bDontLight)
                {
                    // Override the colour...
                    glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
                }
                else
                {
                    // DON'T override the colour
                    glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
                }


                // Wireframe
                if (constrMesh.bIsWireframe || gWireframeMode)                // GL_POINT, GL_LINE, and GL_FILL)
                {
                    // Draw everything with only lines
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }




                // glPointSize(20.0f); sometimes this works... Hey, it's not my fault!



                sModelDrawInfo modelInfo;
                //        if (gVAOManager->FindDrawInfoByModelName("bun_zipper_res2 (justXYZ).ply", modelInfo))
                //        if (gVAOManager->FindDrawInfoByModelName("Assembled_ISS.ply", modelInfo))

                if (gVAOManager.FindDrawInfoByModelName(constrMesh.meshName, modelInfo))
                {
                    glBindVertexArray(modelInfo.VAO_ID);

                    glDrawElements(GL_TRIANGLES,
                        modelInfo.numberOfIndices,
                        GL_UNSIGNED_INT,
                        (void*)0);

                    glBindVertexArray(0);
                }

            }//for end

            for (unsigned int index = 0; index != gameObjects.size(); index++)
            {
                //rendering nonsense

                cGameOjbect* currentObject = gameObjects[index];
                if (currentObject->GetMesh()->meshName == "Sphere_xyz_n_rgba.ply")
                    if (gDebugSphereEnabled)
                        currentObject->GetParticle()->SetPosition(cameraTarget);
                    else continue;


                cMesh* curMesh = currentObject->GetMesh();


                //DrawObject(
                //    curMesh, glm::mat4(1.0f),
                //    matModel_Location, matModelInverseTranspose_Location,
                //    program, gVAOManager);

                matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
                //mat4x4_identity(m);


                // *****************************************************
                // Translate or "move" the object somewhere
                glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                    curMesh->positionXYZ);

                //matModel = matModel * matTranslate;
                // *****************************************************


                // *****************************************************
                // Rotation around the Z axis
                glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
                    curMesh->orientationXYZ.z,//(float)glfwGetTime(),
                    glm::vec3(0.0f, 0.0f, 1.0f));

                //matModel = matModel * rotateZ;
                // *****************************************************

                // *****************************************************
                // Rotation around the Y axis
                glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
                    curMesh->orientationXYZ.y,
                    glm::vec3(0.0f, 1.0f, 0.0f));

                //matModel = matModel * rotateY;
                // *****************************************************

                // *****************************************************
                // Rotation around the X axis
                glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
                    curMesh->orientationXYZ.x,
                    glm::vec3(1.0f, 0.0f, 0.0f));

                //matModel = matModel * rotateX;
                // *****************************************************


                // *****************************************************
                // Scale the model
                glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                    glm::vec3(curMesh->scale,  // Scale in X
                        curMesh->scale,  // Scale in Y
                        curMesh->scale));// Scale in Z

//matModel = matModel * matScale;
// *****************************************************

// *****************************************************
                matModel = matModel * matTranslate;
                matModel = matModel * rotateZ;
                matModel = matModel * rotateY;
                matModel = matModel * rotateX;
                matModel = matModel * matScale;     // <-- mathematically, this is 1st

                // Now the matModel ("Model" or "World") matrix
                //  represents ALL the transformations we want, in ONE matrix.

                p = glm::perspective(0.6f,
                    ratio,
                    0.1f,
                    1000.0f);

                v = glm::mat4(1.0f);

                //glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
                glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

                v = glm::lookAt(cameraEye,     // "eye"
                    cameraTarget,  // "at"
                    upVector);
                //        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);

                //        mvp = p * v * matModel;
                //        mat4x4_mul(mvp, p, m);

                glUseProgram(program);

                //          glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
                    //        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);

                            // Don't need this anymore as it's being done inside the shader:
                            // 
                            // mvp = p * v * matModel;
                            //
                glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));
                glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(v));
                glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(p));

                // Inverse transpose of the model matrix
                // (Used to calculate the normal location in vertex space, using only rotation)
                glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
                glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));



                // Michael won't push this until later...
                GLint bUseDebugColour_Location = glGetUniformLocation(program, "bUseDebugColour");
                GLint objectDebugColour_Location = glGetUniformLocation(program, "objectDebugColour");
                GLint bDontLightObject_Location = glGetUniformLocation(program, "bDontLightObject");


                // See if mesh is wanting the vertex colour override (HACK) to be used?
                if (curMesh->bUseObjectDebugColour)
                {
                    // Override the colour...
                    glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
                    glUniform4f(objectDebugColour_Location,
                        curMesh->objectDebugColourRGBA.r,
                        curMesh->objectDebugColourRGBA.g,
                        curMesh->objectDebugColourRGBA.b,
                        curMesh->objectDebugColourRGBA.a);
                }
                else
                {
                    // DON'T override the colour
                    glUniform1f(bUseDebugColour_Location, (float)GL_FALSE);
                }


                // See if mesh is wanting the vertex colour override (HACK) to be used?
                if (curMesh->bDontLight)
                {
                    // Override the colour...
                    glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
                }
                else
                {
                    // DON'T override the colour
                    glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
                }


                // Wireframe
                if (curMesh->bIsWireframe || gWireframeMode)                // GL_POINT, GL_LINE, and GL_FILL)
                {
                    // Draw everything with only lines
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                }
                else
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                }




                // glPointSize(20.0f); sometimes this works... Hey, it's not my fault!



                sModelDrawInfo modelInfo;
                //        if (gVAOManager->FindDrawInfoByModelName("bun_zipper_res2 (justXYZ).ply", modelInfo))
                //        if (gVAOManager->FindDrawInfoByModelName("Assembled_ISS.ply", modelInfo))

                if (gVAOManager.FindDrawInfoByModelName(gameObjects[index]->GetMesh()->meshName, modelInfo))
                {
                    glBindVertexArray(modelInfo.VAO_ID);

                    glDrawElements(GL_TRIANGLES,
                        modelInfo.numberOfIndices,
                        GL_UNSIGNED_INT,
                        (void*)0);

                    glBindVertexArray(0);
                }


            }//for (unsigned int index



            // Scene is drawn


            // Do physics things
            g_world->TimeStep(deltaTime);
            //g_world->ClearDeadParticles();

            // "Present" what we've drawn.
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwDestroyWindow(window);

        delete g_world;

        glfwTerminate();
        exit(EXIT_SUCCESS);
    
}


void CreateProjectile(std::string objectName)
{
    int randomIndex = (getRandom() * createPositions.size());
    builder.createAtOffset(createPositions[randomIndex], objects[objectName], gameObjects, g_world, g_constraints, g_gravityGenerator);
    builder.applyForceToToy(getRandomVector(10.f), builder.sphereCollections.size() - 1);

   /* glm::vec3 initForce = getDirectionFromRotation(cannon->GetMesh()->orientationXYZ, glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    
    initForce;

    glm::vec3 initPos = cannon->GetMesh()->positionXYZ;

    cMesh* testMeshBall1 = new cMesh();
    testMeshBall1->meshName = "assets/Sphere_xyz_n_rgba.ply";
    testMeshBall1->bUseObjectDebugColour = true;
    testMeshBall1->objectDebugColourRGBA = glm::vec4((getRandom() * 100 + 150)  / 255.f, (getRandom() * 25 + 100) / 255.f, (getRandom() * 50 + 150) / 255.f, 1.f);
    testMeshBall1->positionXYZ = initPos;

    float sizeMassMod = getRandom() + 0.75f;
    testMeshBall1->scale *= sizeMassMod;
    cParticle* testParticleBall1 = new cParticle(0.5f * sizeMassMod, initPos, 1.0f * sizeMassMod);
    testParticleBall1->SetShape(Sphere);
    testParticleBall1->ApplyForce(initForce * 5000.f);

    cGameOjbect* testBall1 = new cGameOjbect(testMeshBall1, testParticleBall1);
    gameObjects.push_back(testBall1);
    g_world->AddParticle(testParticleBall1);
    g_world->GetForceRegistry()->Register(testParticleBall1, g_gravityGenerator);*/

}


/*for (int i = 0; i < g_constraints.size(); i++)
        {
            cParticleRodConstraint* constr = g_constraints[i];
            cMesh constrMesh;
            constrMesh.positionXYZ = (constr->GetParticleA()->GetPosition() + constr->GetParticleB()->GetPosition()) / 2.f;
            constrMesh.meshName = "assets/rodY.ply";
            constrMesh.scale = 0.01f;
            constrMesh.bUseObjectDebugColour = true;
            constrMesh.objectDebugColourRGBA = glm::vec4(0.4f, 0.4f, 0.4f, 1.f);
            glm::vec3 from = constr->GetParticleA()->GetPosition();
            glm::vec3 to = (constr->GetParticleB()->GetPosition());

            glm::vec3 crossthing = cross(from, to);
            float angle = acos(dot(to, from) / (length(to) * length(from)));
            glm::mat4 matRotation = rotate(glm::mat4(1.f), -angle, normalize(crossthing));

            glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                glm::vec3(constrMesh.scale,  // Scale in X
                    constrMesh.scale,  // Scale in Y
                    constrMesh.scale));// Scale in Z

            glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                constrMesh.positionXYZ);

            glm::mat4 matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
            matModel = matModel * matTranslate;

            matModel = matModel * matRotation;
            
            matModel = matModel * matScale;
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            float ratio = width / (float)height;

            glm::mat4 p = glm::perspective(0.6f,
                ratio,
                0.1f,
                1000.0f);

            glm::mat4 v = glm::mat4(1.0f);

            //glm::vec3 cameraEye = glm::vec3(0.0, 0.0, -4.0f);
            glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

            v = glm::lookAt(cameraEye,     // "eye"
                cameraTarget,  // "at"
                upVector);
            //        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);

            //        mvp = p * v * matModel;
            //        mat4x4_mul(mvp, p, m);

            glUseProgram(program);

            //          glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
                //        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);

                        // Don't need this anymore as it's being done inside the shader:
                        // 
                        // mvp = p * v * matModel;
                        //
            glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));
            glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(v));
            glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(p));

            // Inverse transpose of the model matrix
            // (Used to calculate the normal location in vertex space, using only rotation)
            glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
            glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));



            // Michael won't push this until later...
            GLint bUseDebugColour_Location = glGetUniformLocation(program, "bUseDebugColour");
            GLint objectDebugColour_Location = glGetUniformLocation(program, "objectDebugColour");
            GLint bDontLightObject_Location = glGetUniformLocation(program, "bDontLightObject");


            // See if mesh is wanting the vertex colour override (HACK) to be used?
            if (constrMesh.bUseObjectDebugColour)
            {
                // Override the colour...
                glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
                glUniform4f(objectDebugColour_Location,
                    constrMesh.objectDebugColourRGBA.r,
                    constrMesh.objectDebugColourRGBA.g,
                    constrMesh.objectDebugColourRGBA.b,
                    constrMesh.objectDebugColourRGBA.a);
            }
            else
            {
                // DON'T override the colour
                glUniform1f(bUseDebugColour_Location, (float)GL_FALSE);
            }


            // See if mesh is wanting the vertex colour override (HACK) to be used?
            if (constrMesh.bDontLight)
            {
                // Override the colour...
                glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
            }
            else
            {
                // DON'T override the colour
                glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
            }


            // Wireframe
            if (constrMesh.bIsWireframe || gWireframeMode)                // GL_POINT, GL_LINE, and GL_FILL)
            {
                // Draw everything with only lines
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }




            // glPointSize(20.0f); sometimes this works... Hey, it's not my fault!



            sModelDrawInfo modelInfo;
            //        if (gVAOManager->FindDrawInfoByModelName("bun_zipper_res2 (justXYZ).ply", modelInfo))
            //        if (gVAOManager->FindDrawInfoByModelName("Assembled_ISS.ply", modelInfo))

            if (gVAOManager->FindDrawInfoByModelName(constrMesh.meshName, modelInfo))
            {
                glBindVertexArray(modelInfo.VAO_ID);

                glDrawElements(GL_TRIANGLES,
                    modelInfo.numberOfIndices,
                    GL_UNSIGNED_INT,
                    (void*)0);

                glBindVertexArray(0);
            }

        }//for end
*/
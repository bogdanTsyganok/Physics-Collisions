#pragma once
#include "glmIncludes.h"

/*
* author:	Bogdan Tsyganok, 0886354
* purpose:	Helper methods for generating forces and coords
*/

// Generate a random number between 0 and 1
float getRandom();

// Generate a random number between zero and a given high value
float getRandom(float high);

// Generate a random number in a given range
float getRandom(float low, float high);

// Get Randomly pointed vector in any direction
glm::vec3 getRandomVector(float length);

//Get Random Corona-shaped vector
glm::vec3 getRandomCircularVector(float length, float alpha);

//Get Orthonormal basis based on 2 vectors
glm::mat3 orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec);

//Normalize a vector
glm::vec3 normalize(glm::vec3 vec);

//Translate euler rotation to direction
glm::vec3 getDirectionFromRotation(glm::vec3 orientation, glm::vec4 absDirection);

glm::vec3 getOrientationFromDirection(glm::vec3 direction);
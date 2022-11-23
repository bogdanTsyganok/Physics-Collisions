#include "helpers.h"
#include <cstdlib>


float getRandom()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float getRandom(float high)
{
	return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / high);
}

float getRandom(float low, float high)
{
	return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
}

glm::vec3 getRandomVector(float length)
{
	float alpha = getRandom(glm::pi<float>());
	float delta = getRandom(-glm::pi<float>(), glm::pi<float>());
	float sinA = glm::sin(alpha);
	float cosA = glm::cos(alpha);
	float sinD = glm::sin(delta);
	float cosD = glm::cos(delta);
	return glm::vec3(length * sinA * cosD, length * sinA * sinD, length * cosA);
}

glm::vec3 getRandomCircularVector(float length, float alpha)
{
	float delta = getRandom(-glm::pi<float>(), glm::pi<float>());
	float sinA = glm::sin(alpha);
	float cosA = glm::cos(alpha);
	float sinD = glm::sin(delta);
	float cosD = glm::cos(delta);
	return glm::vec3(length * sinA * cosD, length * cosA , length * sinA * sinD);
}

glm::mat3 orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec)
{

	// Generate y, by crossing z and x.
	glm::vec3 x(xVec);
	glm::vec3 z(zVec);
	glm::vec3 y(glm::cross(z, x));
	// Ensure z is orthogonal to both x and y.
	z = glm::cross(x, y);
	// Normalize everything.
	x = glm::normalize(x);
	y = glm::normalize(y);
	z = glm::normalize(z);
	// Return the result.
	return glm::mat3(x, y, z);
}

glm::vec3 normalize(glm::vec3 vec)
{
	float initLength = vec.length();
	if (initLength == 0)
		return vec;
	vec.x /= initLength;
	vec.y /= initLength;
	vec.z /= initLength;
	return vec;
}


glm::vec3 getDirectionFromRotation(glm::vec3 orientation, glm::vec4 absDirection)
{
	//Solution derived from glm::eulerAngleXYZ
	//I am not sure why this works, so I am not going to use that
	//I figured it out
	glm::mat4 orieantationMAT = glm::eulerAngleXYZ(orientation.x, orientation.y, orientation.z);

	glm::vec4 initForce4 = absDirection; //Setting initial direction

	glm::vec3 initForce = initForce4 * orieantationMAT; //??????????
	//This happens because the way I rotate my tube it's coming out "From the wrong end"
	initForce.y = -initForce.y;
	initForce.x = -initForce.x;



	//My attempt to use angles to calculate it
	//using namespace glm;

	////assume end vector is normalized
	//vec3 initForce;
	//float alphaCos = cos(orientation.x);
	//float deltaCos = cos(orientation.y);
	//float deltaSin = sin(orientation.y);
	//initForce.y = -sin(orientation.x); // Opposite of the where end vector should be
	////-sin because I made unfortunate decisions as to where the rear end of the cannon is...
	////The angle of pitch is going to range between 0 and a negative value which produces a negative sin (cos doesn't care, it only goes negative over 90 and under 270 degrees)
	////
	////alphaCos is 1 * alphaCos which is projection of force on surface
	////which one is z and which one is x has been realized experimentally
	//initForce.z = alphaCos * deltaCos; 
	//initForce.x = alphaCos * deltaSin;


	return	normalize(initForce);//normalize just in case
}

inline glm::mat4 rotation_from_to(const glm::vec3& from, const glm::vec3& to)
{
	glm::vec3 v = cross(from, to);
	float angle = acos(dot(to, from) / (length(to) * length(from)));
	return rotate(glm::mat4(1.f), angle, normalize(v));
}

glm::vec3 getOrientationFromDirection(glm::vec3 direction)
{
	using namespace glm;

	glm::vec3 orient;
	//assume end vector is normalized
	//vec3 initForce;
	//float alphaCos = cos(orientation.x);
	//float deltaCos = cos(orientation.y);
	//float deltaSin = sin(orientation.y);
	//initForce.y = -sin(orientation.x); // Opposite of the where end vector should be
	//-sin because I made unfortunate decisions as to where the rear end of the cannon is...
	//The angle of pitch is going to range between 0 and a negative value which produces a negative sin (cos doesn't care, it only goes negative over 90 and under 270 degrees)
	//
	//alphaCos is 1 * alphaCos which is projection of force on surface
	//which one is z and which one is x has been realized experimentally
	/*initForce.z = alphaCos * deltaCos; 
	initForce.x = alphaCos * deltaSin;*/
	orient.y = glm::asin(-direction.x);
	float deltaCos = cos(orient.y);
	float deltaSin = sin(orient.y);
	(direction.z / deltaCos);

	return	normalize(orient);//normalize just in case
}

#pragma once

#include "GameObject.h"
#include "ParticleConstraints.h"

class cPinballToy
{
protected:
	std::vector<glm::vec4> mObjects;
	std::vector<std::pair<int, int>> mConstraints;
public:
	cPinballToy();

	void addObject(glm::vec4 obj);

	void addRodConstraint(int index0, int index1);

	std::vector<glm::vec4> getObjects();
	std::vector<std::pair<int, int>> getConstraints();
};
#include "PinballToys.h"

cPinballToy::cPinballToy()
{
}

void cPinballToy::addObject(glm::vec4 obj)
{
	mObjects.push_back(obj);
}

void cPinballToy::addRodConstraint(int index0, int index1)
{
	if (index0 >= mObjects.size() || index1 >= mObjects.size())
		return;
	mConstraints.push_back(std::make_pair(index0, index1));
}

std::vector<glm::vec4> cPinballToy::getObjects()
{
	return mObjects;
}

std::vector<std::pair<int, int>> cPinballToy::getConstraints()
{
	return mConstraints;
}

#include <iostream>
#include "selectableObject.h"

void SelectableObject::setFunction(std::function<void()> func)
{
	this->storedFunction = func;
}

void SelectableObject::executeFunction()
{
	if (storedFunction) {
		storedFunction();
	}
}

void SelectableObject::moveUp()
{
	this->setYPosition(this->getPosition().y + 0.5f);
}

void SelectableObject::setId(int newId)
{
	this->id = newId;
}

int SelectableObject::getId()
{
	return this->id;
}

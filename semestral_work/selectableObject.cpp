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

void SelectableObject::setId(int newId)
{
	this->id = newId;
}

int SelectableObject::getId()
{
	return this->id;
}

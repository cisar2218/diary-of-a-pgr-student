#include <iostream>
#include "selectableObject.h"

/**
 * \brief Set reference function that will be executed on select event.
 *
 * \param Function to be executed.
 */
void SelectableObject::setFunction(std::function<void()> func)
{
	this->storedFunction = func;
}

/**
 * \brief Execute setted function if any.
 */
void SelectableObject::executeFunction()
{
	if (storedFunction) {
		storedFunction();
	}
}

/**
 * \brief Move this object up by small distance
 */
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

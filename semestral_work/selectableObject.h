#pragma once

#include <iostream>
#include "singlemesh.h"
#include <functional>

using namespace std;

class SelectableObject : public SingleMesh
{
public:
	using SingleMesh::SingleMesh;
	
	void setFunction(std::function<void()> func);
	void executeFunction();
	void moveUp();

	void setId(int newId);
	int getId();
private:
	std::function<void()> storedFunction;
	int id = -1;
};

#pragma once

#include "object.h"


class Sphere : public ObjectInstance
{
public:

	Sphere(ShaderProgram* shdrPrg = nullptr);
	~Sphere();

};
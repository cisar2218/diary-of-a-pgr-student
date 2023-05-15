#pragma once

#include <random>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "pgr.h"
#include "selectableObject.h"

using namespace std;

void printErrIfNotSatisfied(const bool condition, const std::string& errMessage);

// startup utils -----------------------------------------------------------------
shared_ptr<SingleMesh> initSofa(ShaderProgram* shaderProgram);
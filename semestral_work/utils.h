#pragma once

#include <random>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "pgr.h"
#include "selectableObject.h"
#include "dynamicMesh.h"
#include "meshMovTex.h"

using namespace std;

void printErrIfNotSatisfied(const bool condition, const std::string& errMessage);

// startup utils -----------------------------------------------------------------
shared_ptr<SingleMesh> initSofa(ShaderProgram* shaderProgram, GLint texture = -1);
shared_ptr<SingleMesh> initTower(ShaderProgram* shaderProgram, GLint texture=-1);
shared_ptr<SingleMesh> initRailing(ShaderProgram* shaderProgram, GLint texture = -1);
shared_ptr<DynamicMesh> initPlane(ShaderProgram* shaderProgram, GLint texture = -1);
shared_ptr<MeshMovTex> initMovTexObj(ShaderProgram* shaderProgram, GLint texture = -1);

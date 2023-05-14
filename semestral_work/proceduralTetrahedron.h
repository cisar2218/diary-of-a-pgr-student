#pragma once

#include <iostream>
#include "singlemesh.h"

using namespace std;

class Tetrahedron : public ObjectInstance
{
public:
	Tetrahedron(ShaderProgram* shdrPrg = nullptr);
	~Tetrahedron();
private:
    void generateVetices();
    static const int numAttribsPerVertex = 8;
    static const int numVertices = 4;
    float vertices[numVertices * numAttribsPerVertex];
    const unsigned faceIndexes[12] = {
        0, 1, 2,  // Face 1
        0, 1, 3,  // Face 2
        0, 2, 3,  // Face 3
        1, 2, 3   // Face 4
    };

    const glm::vec3 directions[4] = {
        glm::vec3(1, 1, 1),
        glm::vec3(-1, -1, 1),
        glm::vec3(-1, 1, -1),
        glm::vec3(1, -1, -1)
    };
};
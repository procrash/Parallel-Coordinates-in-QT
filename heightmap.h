#pragma once

//#include <glm/vec3.hpp>
#include "./3rdParty/glm/glm/vec3.hpp"
#include <vector>

using namespace std;

class HeightMap
{
private:
    glm::vec3** vVertexData;

public:
    HeightMap();
    void calculateTriangleNormals();
};

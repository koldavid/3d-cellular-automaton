#pragma once

#include "SimpleMesh.h"

class Cube : public SimpleMesh
{
public:
	Cube();
	~Cube();

private:
	static const std::vector<glm::vec3> vertices;
	static const std::vector<glm::vec3> normals;
	static const std::vector<GLuint> indices;
};


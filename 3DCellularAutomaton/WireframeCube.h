#pragma once
#include "SimpleMesh.h"
class WireframeCube : public SimpleMesh
{
public:
	WireframeCube();
	~WireframeCube();

private:
	static const std::vector<glm::vec3> vertices;
	static const std::vector<glm::vec3> normals;
	static const std::vector<GLuint> indices;
};


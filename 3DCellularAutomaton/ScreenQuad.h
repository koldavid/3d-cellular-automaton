#pragma once

#include <glbinding\gl\gl.h>

#include <glm\glm.hpp>
#include <vector>

#include "SimpleMesh.h"

using namespace gl;

class ScreenQuad : public SimpleMesh
{
public:
	ScreenQuad();

private:
	static const std::vector<glm::vec3> vertices;
	static const std::vector<glm::vec3> normals;
	static const std::vector<GLuint> indices;
};


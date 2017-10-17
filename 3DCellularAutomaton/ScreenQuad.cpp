#include "ScreenQuad.h"

#include <glm\glm.hpp>
#include <vector>


const std::vector<glm::vec3> ScreenQuad::vertices{ 
	{ -1.0, -1.0, 0.0 },
	{ 1.0, -1.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
	{ -1.0, 1.0, 0.0 } };

const std::vector<glm::vec3> ScreenQuad::normals{ 
	{ 0.0, 0.0, 1.0 },
	{ 0.0, 0.0, 1.0 },
	{ 0.0, 0.0, 1.0 },
	{ 0.0, 0.0, 1.0 } };

const std::vector<GLuint> ScreenQuad::indices{ 
	0, 1, 2, 
	0, 2, 3 };


ScreenQuad::ScreenQuad() : 
	SimpleMesh(vertices,normals,indices)
{
}

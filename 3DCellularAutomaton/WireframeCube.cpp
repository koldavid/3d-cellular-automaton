#include "WireframeCube.h"

#include <glbinding\gl\gl.h>
using namespace gl;

const std::vector<glm::vec3> WireframeCube::vertices{
	{ -1.0, 1.0, 1.0 },    // 0
	{ 1.0, 1.0, 1.0 },     // 1
	{ 1.0, 1.0, -1.0 },    // 2
	{ -1.0, 1.0, -1.0 },   // 3
	{ -1.0, -1.0, 1.0 },   // 4
	{ 1.0, -1.0, 1.0 },    // 5
	{ 1.0, -1.0, -1.0 },   // 6
	{ -1.0, -1.0, -1.0 } };// 7

const std::vector<glm::vec3> WireframeCube::normals{
	{ -1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 1.0, 1.0, -1.0 },
	{ -1.0, 1.0, -1.0 },
	{ -1.0, -1.0, 1.0 },
	{ 1.0, -1.0, 1.0 },
	{ 1.0, -1.0, -1.0 },
	{ -1.0, -1.0, -1.0 } };

const std::vector<GLuint> WireframeCube::indices{
	0, 1, // I
	1, 2, // II
	2, 3, // III
	3, 0, // IV
	4, 5, // V
	5, 6, // VI
	6, 7, // VII
	7, 4, // VIII
	0, 4, // IX
	1, 5, // X
	2, 6, // XI
	3, 7 };// XII

WireframeCube::WireframeCube() : 
	SimpleMesh(vertices, normals, indices, GL_LINES)
{
}


WireframeCube::~WireframeCube()
{
}

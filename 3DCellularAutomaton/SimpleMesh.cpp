#include "SimpleMesh.h"

#include <glbinding\gl\gl.h>

using namespace gl;

SimpleMesh::SimpleMesh()
{
}

SimpleMesh::SimpleMesh(
	const std::vector<glm::vec3>& vertices, 
	const std::vector<glm::vec3>& normals, 
	const std::vector<gl::GLuint>& indices,
	GLenum drawMode) :
	vaoID(0), 
	numIndices(static_cast<GLuint>(indices.size())), 
	drawMode(drawMode)
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(3, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[BufferPos::vertex]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[BufferPos::normal]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[BufferPos::index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}


SimpleMesh::~SimpleMesh()
{
	glDeleteVertexArrays(1, &vaoID);
}

void SimpleMesh::render() const
{
	glBindVertexArray(vaoID);

	glDrawElements(drawMode, numIndices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void SimpleMesh::renderInstanced(GLuint numInstances) const
{
	glBindVertexArray(vaoID);

	glDrawElementsInstanced(drawMode, numIndices, GL_UNSIGNED_INT, 0, numInstances);

	glBindVertexArray(0);
}

#pragma once

#include <glm\glm.hpp>
#include <glbinding\gl\gl.h>
#include <vector>

using namespace gl;

class SimpleMesh
{
public:
	SimpleMesh();

	SimpleMesh(
		const std::vector<glm::vec3>& vertices,
		const std::vector<glm::vec3>& normals,
		const std::vector<GLuint>& indices,
		GLenum drawMode = GL_TRIANGLES);

	~SimpleMesh();

	void render() const;
	void renderInstanced(GLuint numInstances) const;

private:
	GLuint vaoID;
	GLuint buffers[3];

	GLuint numIndices;

	const GLuint numBuffers = 3;
	enum BufferPos {
		vertex = 0,
		normal = 1,
		index = 2
	};

	GLenum drawMode;
};


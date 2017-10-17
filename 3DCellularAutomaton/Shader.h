#pragma once

#include <string>
#include <glbinding\gl\gl.h>

using namespace gl;

class Shader
{
public:
	Shader(const GLenum shaderType, const std::string& source);
	Shader(const GLenum shaderType, const std::istream& source);

	~Shader();

	GLuint getId() const;

	static Shader* fromFile(const GLenum shaderType, const std::string& fileName);

private:

	void compile(const std::string& source);

	static std::string shaderTypeToString(const GLenum shaderType);

	GLuint shaderId;
	GLenum shaderType;
};
#include "Shader.h"

#include <glbinding\gl\gl.h>
#include <glbinding\gl\enum.h>

#include <iostream>
#include <fstream>
#include <sstream>

using namespace gl;

Shader::Shader(const GLenum shaderType, const std::string & source) :
	shaderId(0), shaderType(shaderType)
{
	shaderId = glCreateShader(shaderType);
	compile(source);
}

Shader::Shader(const GLenum shaderType, const std::istream & source) :
	shaderId(0), shaderType(shaderType)
{
	shaderId = glCreateShader(shaderType);

	std::stringstream ss;
	ss << source.rdbuf();

	compile(ss.str());
}

Shader::~Shader()
{
}

GLuint Shader::getId() const
{
	return shaderId;
}

Shader * Shader::fromFile(const GLenum shaderType, const std::string & fileName)
{
	std::ifstream shaderSrc(fileName);

	if (shaderSrc.is_open()) {
		return new Shader(shaderType, shaderSrc);
	}
	else {
		return nullptr;
	}
}

void Shader::compile(const std::string& source)
{
	const GLchar* p[1];
	p[0] = source.c_str();
	GLint lengths[1];
	lengths[0] = static_cast<GLint>(source.length());
	glShaderSource(shaderId, 1, p, lengths);

	glCompileShader(shaderId);

	GLint success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderId, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error compiling shader type " << shaderTypeToString(shaderType) << ": " << infoLog << std::endl;

		std::cin.get();
	}
}

std::string Shader::shaderTypeToString(const GLenum shaderType)
{
	switch (shaderType) {
	case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER"; break;
	case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER"; break;
	case GL_GEOMETRY_SHADER: return "GL_GEOMETRY_SHADER"; break;
	case GL_TESS_CONTROL_SHADER: return "GL_TESS_CONTROL_SHADER"; break;
	case GL_TESS_EVALUATION_SHADER: return "GL_TESS_EVALUATION_SHADER"; break;
	case GL_COMPUTE_SHADER: return "GL_COMPUTE_SHADER"; break;
	default: return "GL_UNKNOWN_SHADER"; break;
	};
}

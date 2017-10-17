#include "ShaderProgram.h"

#include <iostream>
#include <vector>
#include <glm\gtc\type_ptr.hpp>

ShaderProgram::ShaderProgram() :
	programId(0)
{
	programId = glCreateProgram();
}


ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::attach(Shader * shader)
{
	if (shader != nullptr) {
		glAttachShader(programId, shader->getId());
		shaders.insert(shader);

		// std::cout << "Shader with ID:" << shader->getId() << " attached to program with ID:" << programId << std::endl;
	}
	else {
		std::cerr << "Shader is NULL!" << std::endl;
		std::cin.get();
	}

	this->link();
}

void ShaderProgram::use() const
{
	validate();
	glUseProgram(programId);
}

GLint ShaderProgram::get(const GLenum pname) const
{
	GLint value = 0;
	glGetProgramiv(programId, pname, &value);

	return value;
}

std::string ShaderProgram::infoLog() const
{
	GLint length = get(GL_INFO_LOG_LENGTH);

	if (length == 0)
		return std::string();

	std::vector<char> log(length);

	glGetProgramInfoLog(programId, length, &length, log.data());

	return std::string(log.data(), length);
}

GLuint ShaderProgram::getUniformLocation(const std::string & name) const
{
	this->validate();

	GLuint value = glGetUniformLocation(programId, name.c_str());

	if (value == -1) {
		std::cerr << "Can't get uniform with name: '" << name << "'" << std::endl;
	}

	return value;
}

glm::mat4 ShaderProgram::getUniform(const std::string & name)
{
	GLuint location = getUniformLocation(name);
	glm::mat4 value;
	glGetUniformfv(programId, location, glm::value_ptr(value));

	return value;
}

GLuint ShaderProgram::id() const
{
	return this->programId;
}

void ShaderProgram::link()
{
	glLinkProgram(programId);

	if (static_cast<GLboolean>(get(GL_LINK_STATUS)) == GL_FALSE) 
	{
		std::cerr << "Error linking shader program: " << infoLog() << std::endl;
		std::cin.get();
	}
}

void ShaderProgram::validate() const
{
	glValidateProgram(programId);

	if (get(gl::GL_VALIDATE_STATUS) != 1) {
		std::cout << "Shader program not valid!" << std::endl;
	}
}



void ShaderProgram::setValue(const GLint location, const float & value) const
{
	this->use();
	glUniform1f(location, value);
}

void ShaderProgram::setValue(const GLint location, const int & value) const
{
	this->use();
	glUniform1i(location, value);
}

void ShaderProgram::setValue(const GLint location, const unsigned int & value) const
{
	this->use();
	glUniform1ui(location, value);
}

void ShaderProgram::setValue(const GLint location, const bool & value) const
{
	this->use();
	glUniform1i(location, value ? 1 : 0);
}

void ShaderProgram::setValue(const GLint location, const glm::vec2 & value) const
{
	this->use();
	glUniform2fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::vec3 & value) const
{
	this->use();
	glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::vec4 & value) const
{
	this->use();
	glUniform4fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::ivec2 & value) const
{
	this->use();
	glUniform2iv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::ivec3 & value) const
{
	this->use();
	glUniform3iv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::ivec4 & value) const
{
	this->use();
	glUniform4iv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::uvec2 & value) const
{
	this->use();
	glUniform2uiv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::uvec3 & value) const
{
	this->use();
	glUniform3uiv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::uvec4 & value) const
{
	this->use();
	glUniform4uiv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::mat2 & value) const
{
	this->use();
	glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::mat3 & value) const
{
	this->use();
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::mat4 & value) const
{
	this->use();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::mat2x3 & value) const
{
	this->use();
	glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::mat3x2 & value) const
{
	this->use();
	glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::mat2x4 & value) const
{
	this->use();
	glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::mat4x2 & value) const
{
	this->use();
	glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::mat3x4 & value) const
{
	this->use();
	glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const glm::mat4x3 & value) const
{
	this->use();
	glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setValue(const GLint location, const GLuint64 & value) const
{
	this->use();
	glUniformHandleui64ARB(location, value);
}

void ShaderProgram::setValue(const GLint location, const std::vector<float> & value) const
{
	this->use();
	glUniform1fv(location, static_cast<GLint>(value.size()), value.data());
}

void ShaderProgram::setValue(const GLint location, const std::vector<int> & value) const
{
	this->use();
	glUniform1iv(location, static_cast<GLint>(value.size()), value.data());
}

void ShaderProgram::setValue(const GLint location, const std::vector<unsigned int> & value) const
{
	this->use();
	glUniform1uiv(location, static_cast<GLint>(value.size()), value.data());
}

void ShaderProgram::setValue(GLint location, const std::vector<bool> & value) const
{
	std::vector<int> values(value.size());
	for (unsigned i = 0; i < values.size(); ++i)
	{
		values[i] = value[i] ? 1 : 0;
	}

	this->use();
	glUniform1iv(location, static_cast<GLint>(values.size()), values.data());
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::vec2> & value) const
{
	this->use();
	glUniform2fv(location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::vec3> & value) const
{
	this->use();
	glUniform3fv(location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::vec4> & value) const
{
	this->use();
	glUniform4fv(location, static_cast<GLint>(value.size()), reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::ivec2> & value) const
{
	this->use();
	glUniform2iv(location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::ivec3> & value) const
{
	this->use();
	glUniform3iv(location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::ivec4> & value) const
{
	this->use();
	glUniform4iv(location, static_cast<GLint>(value.size()), reinterpret_cast<const int*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::uvec2> & value) const
{
	this->use();
	glUniform2uiv(location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::uvec3> & value) const
{
	this->use();
	glUniform3uiv(location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::uvec4> & value) const
{
	this->use();
	glUniform4uiv(location, static_cast<GLint>(value.size()), reinterpret_cast<const unsigned*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::mat2> & value) const
{
	this->use();
	glUniformMatrix2fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::mat3> & value) const
{
	this->use();
	glUniformMatrix3fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::mat4> & value) const
{
	this->use();
	glUniformMatrix4fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::mat2x3> & value) const
{
	this->use();
	glUniformMatrix2x3fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::mat3x2> & value) const
{
	this->use();
	glUniformMatrix3x2fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::mat2x4> & value) const
{
	this->use();
	glUniformMatrix2x4fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::mat4x2> & value) const
{
	this->use();
	glUniformMatrix4x2fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::mat3x4> & value) const
{
	this->use();
	glUniformMatrix3x4fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

void ShaderProgram::setValue(const GLint location, const std::vector<glm::mat4x3> & value) const
{
	this->use();
	glUniformMatrix4x3fv(location, static_cast<GLint>(value.size()), GL_FALSE, reinterpret_cast<const float*>(value.data()));
}

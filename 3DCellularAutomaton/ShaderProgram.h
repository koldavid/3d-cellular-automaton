#pragma once

#include <vector>
#include <array>
#include <set>

#include <glbinding\gl\gl.h>
#include <glm\glm.hpp>

#include "Shader.h"

using namespace gl;

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	void attach(Shader* shader);

	template<class ...Shaders>
	void attach(Shader* shader, Shaders... shaders);

	void use() const;

	GLint get(const GLenum pname) const;

	std::string infoLog() const;

	GLuint getUniformLocation(const std::string& name) const;

	glm::mat4 getUniform(const std::string& name);

	GLuint id() const;

	template<typename T>
	void setUniform(GLuint location, T value) const;

	template<typename T>
	void setUniform(const std::string& name, T value) const;

protected:
	void setValue(GLint location, const float & value) const;
	void setValue(GLint location, const int & value) const;
	void setValue(GLint location, const unsigned int & value) const;
	void setValue(GLint location, const bool & value) const;

	void setValue(GLint location, const glm::vec2 & value) const;
	void setValue(GLint location, const glm::vec3 & value) const;
	void setValue(GLint location, const glm::vec4 & value) const;

	void setValue(GLint location, const glm::ivec2 & value) const;
	void setValue(GLint location, const glm::ivec3 & value) const;
	void setValue(GLint location, const glm::ivec4 & value) const;

	void setValue(GLint location, const glm::uvec2 & value) const;
	void setValue(GLint location, const glm::uvec3 & value) const;
	void setValue(GLint location, const glm::uvec4 & value) const;

	void setValue(GLint location, const glm::mat2 & value) const;
	void setValue(GLint location, const glm::mat3 & value) const;
	void setValue(GLint location, const glm::mat4 & value) const;

	void setValue(GLint location, const glm::mat2x3 & value) const;
	void setValue(GLint location, const glm::mat3x2 & value) const;
	void setValue(GLint location, const glm::mat2x4 & value) const;
	void setValue(GLint location, const glm::mat4x2 & value) const;
	void setValue(GLint location, const glm::mat3x4 & value) const;
	void setValue(GLint location, const glm::mat4x3 & value) const;

	void setValue(GLint location, const gl::GLuint64 & value) const;

	void setValue(GLint location, const std::vector<float> & value) const;
	void setValue(GLint location, const std::vector<int> & value) const;
	void setValue(GLint location, const std::vector<unsigned int> & value) const;
	void setValue(GLint location, const std::vector<bool> & value) const;

	void setValue(GLint location, const std::vector<glm::vec2> & value) const;
	void setValue(GLint location, const std::vector<glm::vec3> & value) const;
	void setValue(GLint location, const std::vector<glm::vec4> & value) const;

	void setValue(GLint location, const std::vector<glm::ivec2> & value) const;
	void setValue(GLint location, const std::vector<glm::ivec3> & value) const;
	void setValue(GLint location, const std::vector<glm::ivec4> & value) const;

	void setValue(GLint location, const std::vector<glm::uvec2> & value) const;
	void setValue(GLint location, const std::vector<glm::uvec3> & value) const;
	void setValue(GLint location, const std::vector<glm::uvec4> & value) const;

	void setValue(GLint location, const std::vector<glm::mat2> & value) const;
	void setValue(GLint location, const std::vector<glm::mat3> & value) const;
	void setValue(GLint location, const std::vector<glm::mat4> & value) const;

	void setValue(GLint location, const std::vector<glm::mat2x3> & value) const;
	void setValue(GLint location, const std::vector<glm::mat3x2> & value) const;
	void setValue(GLint location, const std::vector<glm::mat2x4> & value) const;
	void setValue(GLint location, const std::vector<glm::mat4x2> & value) const;
	void setValue(GLint location, const std::vector<glm::mat3x4> & value) const;
	void setValue(GLint location, const std::vector<glm::mat4x3> & value) const;

	void setValue(GLint location, const std::vector<gl::GLuint64> & value) const;

	template <typename T, std::size_t Count>
	void setValue(GLint location, const std::array<T, Count> & value) const;

private:

	void validate() const;
	void link();

	GLuint programId;

	std::set<Shader*> shaders;
};

#include "ShaderProgram.inl"

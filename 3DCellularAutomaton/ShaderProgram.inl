#pragma once

#include "ShaderProgram.h"

template<class ...Shaders>
inline void ShaderProgram::attach(Shader * shader, Shaders ...shaders)
{
	attach(shader);
	attach(shaders...);
}

template<typename T>
inline void ShaderProgram::setUniform(GLuint location, T value) const
{
	setValue(location, value);
}

template<typename T>
inline void ShaderProgram::setUniform(const std::string & name, T value) const
{
	GLuint location = getUniformLocation(name);
	setUniform(location, value);
}

template<typename T, std::size_t Count>
inline void ShaderProgram::setValue(GLint location, const std::array<T, Count>& value) const
{
	setValue(location, std::vector<T>(value.data(), value.data() + Count));
}
#pragma once

#include <glm\glm.hpp>
#include <array>

class ColorGradient
{
public:
	ColorGradient();
	ColorGradient(std::string line);
	~ColorGradient();

	std::array<glm::vec3, 11>& data();
	static glm::vec3 parseHexadecimalColor(std::string strColor);

private:

	std::array<glm::vec3, 11> colors;
};


#include "ColorGradient.h"

#include <sstream>
#include <iostream>


ColorGradient::ColorGradient() :
	colors({glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 0.0, 0.0)})
{
}

ColorGradient::ColorGradient(std::string line)
{
	std::stringstream ss(line);

	int index = 0;
	for (std::string color; ss >> color; index++) {
		colors[index] = parseHexadecimalColor(color);
	}
}


ColorGradient::~ColorGradient()
{
}

std::array<glm::vec3, 11>& ColorGradient::data()
{
	return colors;
}

glm::vec3 ColorGradient::parseHexadecimalColor(std::string strColor)
{
	if (strColor[0] == '#') {
		strColor = strColor.substr(1, 6);
	}

	int color = std::strtol(strColor.c_str(), NULL, 16);

	return glm::vec3(
		((color & 0xFF0000) >> 16)/256.0f, 
		((color & 0x00FF00) >> 8)/256.0f, 
		(color & 0x0000FF)/256.0f);
}


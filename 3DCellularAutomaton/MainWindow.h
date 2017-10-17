#pragma once

#include <vector>
#include <array>

#include <SDL2\SDL.h>

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "ScreenQuad.h"
#include "Cube.h"
#include "WireframeCube.h"
#include "CellularAutomaton.h"
#include "ColorGradient.h"

class MainWindow
{
public:
	MainWindow(const int width, const int height);
	~MainWindow();

	MainWindow(const MainWindow&) = delete;
	const MainWindow& operator=(const MainWindow&) = delete;

	bool init();

	void run();

private:
	bool loadSettings(std::string source);
	bool initGL();
	bool initCUDA();

	void processEvents();
	void logic();
	void render();

	void close();

	void updateViewPort(unsigned int width, unsigned height);
	void updateCamera(const glm::vec3& location);
	void renderAsWireframe(bool wireframe, GLfloat lineWidth = 1.0f);

	GLuint initSSBO(GLuint size, const void* data, GLenum usage = GL_DYNAMIC_DRAW);
	void bindSSBO(GLuint layoutLocation, const int* data = nullptr, size_t size = 0) const;
	void unBindSSBO() const;

	void changeRule(int ruleNumber);

	CellularAutomaton* cellularAutomata;

	// settings
	int windowWidth;
	int windowHeight;
	int antialiassing;

	int cubeDimension;
	int initialCubeSize;
	float initialCubeDensity;

	glm::vec3 backgroundColor;
	glm::vec3 boundingBoxColor;
	float boundingBoxTransparency;
	glm::vec3 wireframeBoundingBoxColor;

	std::vector<ColorGradient> colorGradients;
	std::vector<CellularAutomaton::Rule> rulesets;

	// rendering
	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_GLContext glContext;
	ShaderProgram* triangleShader;
	ShaderProgram* lineShader;
	ShaderProgram* boundingCube_shader;
	GLuint ssbo;
	bool cudaSupported;

	// meshes
	ScreenQuad* quad;
	Cube* cube;
	WireframeCube* wireframeCube;
	Cube* boundingCube;
	WireframeCube* wireframeBoundingCube;

	// matrices
	glm::mat4 projectionM;

	glm::mat4 translateM;
	glm::mat4 rotateM;
	glm::mat4 scaleM;

	glm::mat4 worldM;

	glm::mat4 viewM;
	glm::vec3 eyePos;

	// event handling
	bool isMouseButtonDown;
	bool isMouseMoving;
	glm::vec2 clickPos;
	glm::vec2 motionPos;
	float zoomFactor;
	bool isCalculating;
	bool isRunning;
	int colorTheme;
	Uint32 updateFrequency;
	Uint32 lastUpdate;
	Uint32 maxFPS;
	Uint32 lastFrame;
};


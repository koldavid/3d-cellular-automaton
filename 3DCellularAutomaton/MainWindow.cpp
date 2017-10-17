#include "MainWindow.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glbinding\gl\gl.h>

#pragma warning(disable: 4251)
#include <glbinding\Binding.h>
#pragma warning(default: 4251)

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\string_cast.hpp>

#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>

#include <inih\INIReader.h>

#include "Shader.h"


using namespace gl;

MainWindow::MainWindow(const int width, const int height) :
	windowWidth(width), windowHeight(height),
	isRunning(true),
	window(nullptr), screenSurface(nullptr), glContext(NULL),
	triangleShader(nullptr), quad(nullptr),
	isMouseButtonDown(false), isMouseMoving(false),
	isCalculating(false), cudaSupported(true),
	updateFrequency(1), lastUpdate(0),
	maxFPS(100), lastFrame(0)
{
}


MainWindow::~MainWindow()
{
}


bool MainWindow::init()
{
	if (!loadSettings("settings.ini")) {
		std::cerr << "Loading settings failed!" << std::endl;
		return false;
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, antialiassing);

	window = SDL_CreateWindow("3D Cellular Automaton", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		windowWidth, windowHeight, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (window == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	screenSurface = SDL_GetWindowSurface(window);

	glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL) {
		std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (SDL_GL_SetSwapInterval(1) < 0) {
		std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
	}

	if (!initGL())
	{
		std::cerr << "Unable to initialize OpenGL!" << std::endl;
		return false;
	}

	if (!initCUDA())
	{
		std::cerr << "Unable to initialize CUDA!" << std::endl;
		cudaSupported = false;
	}

	quad = new ScreenQuad();
	cube = new Cube();
	wireframeCube = new WireframeCube();
	boundingCube = new Cube();
	wireframeBoundingCube = new WireframeCube();

	cellularAutomata = new CellularAutomaton(cubeDimension, rulesets[0]);
	cellularAutomata->initialize(initialCubeSize, initialCubeDensity);

	ssbo = initSSBO(sizeof(int)*cellularAutomata->getCellCount(), cellularAutomata->getDataPtr());

	scaleM = glm::mat4(1.0f);
	translateM = glm::mat4(1.0f);
	rotateM = glm::mat4(1.0f);

	viewM = glm::mat4(1.0f);
	zoomFactor = 5.0f * cellularAutomata->getDimension();

	updateViewPort(windowWidth, windowHeight);
	eyePos = glm::vec3(0.0f, 0.0f, -zoomFactor);
	updateCamera(eyePos);

	return true;
}


void MainWindow::run()
{
	while(isRunning) {
		processEvents();
		logic();
		render();
	}

	close();
}

bool MainWindow::loadSettings(std::string source)
{
	INIReader reader(source);

	windowWidth   = reader.GetInteger("Graphics", "ResolutionX", 800);
	windowHeight  = reader.GetInteger("Graphics", "ResolutionY", 600);
	antialiassing = reader.GetInteger("Graphics", "Antialiassing", 0);
	maxFPS = reader.GetInteger("Graphics", "MaxFPS", 60);

	cubeDimension = reader.GetInteger("CellularAutomaton", "Size", 10);
	initialCubeSize = reader.GetInteger("CellularAutomaton", "InitialCubeSize", 5);
	initialCubeDensity = reader.GetReal("CellularAutomaton", "InitialCubeDensity", 0.5);

	backgroundColor = ColorGradient::parseHexadecimalColor(reader.Get("ColorTheme", "BackgroundColor", "#808080"));
	boundingBoxColor = ColorGradient::parseHexadecimalColor(reader.Get("ColorTheme", "BoundingBoxColor", "#FFFFFFF"));
	boundingBoxTransparency = reader.GetReal("ColorTheme", "BoundingBoxTransparency", 0.25f);
	wireframeBoundingBoxColor = ColorGradient::parseHexadecimalColor(reader.Get("ColorTheme", "WireframeBoundingBoxColor", "FFFFFFF"));
	
	std::string gradient;
	do {
		gradient = reader.Get("ColorTheme", "Gradient_" + std::to_string(colorGradients.size()+1), "");
		if (gradient != "") {
			colorGradients.push_back(ColorGradient(gradient));
		}
	} while (gradient != "");

	std::string name;
	std::string survival;
	std::string birth;
	int numberOfStates;
	do {
		std::string ruleset = "Ruleset_" + std::to_string(rulesets.size() + 1);
		name = reader.Get(ruleset, "Name", "");

		survival = reader.Get(ruleset, "Survival", "");
		std::stringstream ss_survival(survival);
		std::vector<int> v_survival;
		for (int x; ss_survival >> x; ) {
			v_survival.push_back(x);
		}

		birth = reader.Get(ruleset, "Birth", "");
		std::stringstream ss_birth(birth);
		std::vector<int> v_birth;
		for (int x; ss_birth >> x; ) {
			v_birth.push_back(x);
		}

		numberOfStates = reader.GetInteger(ruleset, "NumberOfStates", -1);

		if (name != "" && survival != "" && birth != "" && numberOfStates != -1) {
			rulesets.push_back(CellularAutomaton::Rule(v_survival, v_birth,numberOfStates,name));
		}


	} while (name != "" && survival != "" && birth != "" && numberOfStates != -1);


	return true;
}

bool MainWindow::initGL()
{
	glbinding::Binding::initialize();

	glClearColor(backgroundColor.r,backgroundColor.g,backgroundColor.z, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW); 
	glCullFace(GL_BACK);
	glLineWidth(1.0f);

	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	triangleShader = new ShaderProgram();
	lineShader = new ShaderProgram();
	boundingCube_shader = new ShaderProgram();

	triangleShader->attach(
		Shader::fromFile(GL_VERTEX_SHADER, "vertexshader.glsl"),
		Shader::fromFile(GL_GEOMETRY_SHADER, "geometryshader_triangles.glsl"),
		Shader::fromFile(GL_FRAGMENT_SHADER, "fragmentshader.glsl"));

	lineShader->attach(
		Shader::fromFile(GL_VERTEX_SHADER, "vertexshader.glsl"),
		Shader::fromFile(GL_GEOMETRY_SHADER, "geometryshader_lines.glsl"),
		Shader::fromFile(GL_FRAGMENT_SHADER, "fragmentshader.glsl"));

	boundingCube_shader->attach(
		Shader::fromFile(GL_VERTEX_SHADER, "boundingcube_vertexshader.glsl"),
		Shader::fromFile(GL_FRAGMENT_SHADER, "fragmentshader.glsl"));

	triangleShader->use();
	lineShader->use();
	boundingCube_shader->use();

	return true;
}

bool MainWindow::initCUDA()
{
	int device_count;
	cudaGetDeviceCount(&device_count);

	if (device_count == 0) {
		std::cerr << "CUDA Error: No cuda device found" << std::endl;
		return false;
	}
	else {
		cudaSetDevice(device_count-1);
	}

	return true;
}

void MainWindow::processEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_UP:
						break;

					case SDLK_DOWN:
						break;

					case SDLK_LEFT:
						break;

					case SDLK_RIGHT:
						break;

					case SDLK_SPACE:
						isCalculating = !isCalculating;
						std::cout << (isCalculating ? "Start" : "Stop") << std::endl;
						break;

					case SDLK_KP_PLUS:
						if (updateFrequency > 10) {
							updateFrequency -= 10;
							std::cout << "Update frequency: " << updateFrequency << "ms" << std::endl;
						}
						break;

					case SDLK_KP_MINUS:
						updateFrequency += 10;
						std::cout << "Update frequency: " << updateFrequency << "ms" << std::endl;
						break;

					case SDLK_ESCAPE:
						isRunning = false;
						break;

					case SDLK_c:
						colorTheme++;
						if (colorTheme == colorGradients.size()) {
							colorTheme = 0;
						}
						break;

					case SDLK_1:
						changeRule(0);
						break;
					case SDLK_2:
						changeRule(1);
						break;
					case SDLK_3:
						changeRule(2);
						break;
					case SDLK_4:
						changeRule(3);
						break;
					case SDLK_5:
						changeRule(4);
						break;
					case SDLK_6:
						changeRule(5);
						break;
					case SDLK_7:
						changeRule(6);
						break;
					case SDLK_8:
						changeRule(7);
						break;
					case SDLK_9:
						changeRule(8);
						break;
					case SDLK_0:
						changeRule(9);
						break;

					default:
						break;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				isMouseButtonDown = true;
				isMouseMoving = false;
				clickPos = glm::vec2(event.button.x, event.button.y);
				break;

			case SDL_MOUSEMOTION:
				if (isMouseButtonDown) {
					motionPos = glm::vec2(event.motion.x, event.motion.y);
				}
				isMouseMoving = true;

				break;

			case SDL_MOUSEBUTTONUP:
				isMouseButtonDown = false;
				isMouseMoving = false;
				clickPos = glm::vec2(0.0f);
				motionPos = glm::vec2(0.0f);
				break;

			case SDL_MOUSEWHEEL:
				zoomFactor -= event.wheel.y*3;
				if (zoomFactor < 1.0f) {
					zoomFactor = 1.0f;
				}
				eyePos = glm::vec3(0.0f, 0.0f, -zoomFactor);
				updateCamera(eyePos);


			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						updateViewPort(event.window.data1, event.window.data2);
						break;
				
					default:
						break;
				}
				break;

			default:
				break;
		}
	}
}

				
void MainWindow::logic()
{
	if (isMouseButtonDown && isMouseMoving) {
		float a = motionPos.x - clickPos.x;
		float b = motionPos.y - clickPos.y;
		float c = sqrt(a*a + b*b);

		float radian = atan2(b, a);
		if (radian < 0.0) radian += glm::two_pi<float>();
		float angle = glm::degrees(radian);

		glm::vec3 rotateAxis;

		if (angle >= 0.0 && angle < 90)
		{
			rotateAxis.x = -angle / 90;
			rotateAxis.y = (90 - angle) / 90;
		}
		else {
			if (angle >= 90.0 && angle < 180)
			{
				angle -= 90.0;
				rotateAxis.x = -(90 - angle) / 90;
				rotateAxis.y = -(angle / 90);
			}
			else {
				if (angle >= 180.0 && angle < 270)
				{
					angle -= 180.0;
					rotateAxis.x = angle / 90;
					rotateAxis.y = -(90 - angle) / 90;
				}
				else {
					if (angle >= 270.0 && angle < 360)
					{
						angle -= 270.0;
						rotateAxis.x = (90 - angle) / 90;
						rotateAxis.y = (angle / 90);
					}
				}
			}
		}

		rotateAxis.z = 0.0;
		float rotateAngle = c / 2000;

		glm::mat4 tempRot = rotateM;
		rotateM = glm::rotate(rotateAngle, rotateAxis);
		rotateM *= tempRot;

		//isMouseMoving = false;
	}

	worldM = translateM * rotateM * scaleM;

	if (isCalculating) {
		if (SDL_GetTicks() - lastUpdate > updateFrequency) {
			if (cudaSupported) {
				cellularAutomata->updateCells_CUDA();
			}
			else {
				cellularAutomata->updateCells(); // VERY slow, just a fallback implementation
			}
			lastUpdate = SDL_GetTicks();
		}
	}
}


void MainWindow::render()
{
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glDisable(GL_DEPTH_TEST);
	boundingCube_shader->use();
		boundingCube_shader->setUniform("u_worldM", worldM);
		boundingCube_shader->setUniform("u_viewM", viewM);
		boundingCube_shader->setUniform("u_projectionM", projectionM);
		boundingCube_shader->setUniform("u_cubeDimension", cellularAutomata->getDimension());
		boundingCube_shader->setUniform("u_cubeColor", glm::vec4(boundingBoxColor, boundingBoxTransparency));
		boundingCube_shader->setUniform("u_wireframeColor", glm::vec4(wireframeBoundingBoxColor, 1.0));

		boundingCube_shader->setUniform("u_isWireframe", false);
		boundingCube->render();

		glEnable(GL_DEPTH_TEST);
		boundingCube_shader->setUniform("u_isWireframe", true);
		wireframeBoundingCube->render();


	triangleShader->use();

		triangleShader->setUniform("u_worldM", worldM);
		triangleShader->setUniform("u_viewM", viewM);
		triangleShader->setUniform("u_projectionM", projectionM);
		triangleShader->setUniform("u_numStates", cellularAutomata->getNumberOfStates()-1);
		triangleShader->setUniform("u_cubeDimension", cellularAutomata->getDimension());
		triangleShader->setUniform("u_gradient", colorGradients[colorTheme].data());
		this->bindSSBO(3, cellularAutomata->getDataPtr(), cellularAutomata->getCellCount());

		cube->renderInstanced(cellularAutomata->getCellCount());

	lineShader->use();

		lineShader->setUniform("u_worldM", worldM);
		lineShader->setUniform("u_viewM", viewM);
		lineShader->setUniform("u_projectionM", projectionM);
		lineShader->setUniform("u_cubeDimension", cellularAutomata->getDimension());

		wireframeCube->renderInstanced(cellularAutomata->getCellCount());

		this->unBindSSBO();

		
	if (SDL_GetTicks() - lastFrame > 1000/maxFPS) {
		SDL_GL_SwapWindow(window);
		lastFrame = SDL_GetTicks();
	} 
}


void MainWindow::close()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void MainWindow::updateViewPort(unsigned int width, unsigned height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);

	float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

	projectionM = glm::perspective(glm::quarter_pi<float>(), aspectRatio, 0.1f, 1000.0f);
}

void MainWindow::updateCamera(const glm::vec3 & location)
{
	viewM = glm::lookAt(location, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void MainWindow::renderAsWireframe(bool wireframe, GLfloat lineWidth)
{
	glLineWidth(lineWidth);
	if (wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

GLuint MainWindow::initSSBO(GLuint size, const void * data, GLenum usage)
{
	GLuint ssboID;
	glGenBuffers(1, &ssboID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssboID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return ssboID;
}

void MainWindow::bindSSBO(GLuint layoutLocation, const int* data, size_t size) const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	if (data != nullptr) {
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int)*size, data, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, layoutLocation, ssbo);
	}
}

void MainWindow::unBindSSBO() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void MainWindow::changeRule(int ruleNumber)
{
	if (rulesets.size() > ruleNumber) {
		if (cellularAutomata != nullptr) {
			delete cellularAutomata;
		}
		cellularAutomata = new CellularAutomaton(cubeDimension, rulesets[ruleNumber]);
		cellularAutomata->initialize(initialCubeSize, initialCubeDensity);
		isCalculating = false;
		std::cout << "Stop" << std::endl;
	}
}

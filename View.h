#pragma once

#include "framework.h"
#include "Shape.h"

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

// Manages what is displayed on the screen
class View
{
public:
	// Constructor and deconstructor
	View() { vCamera = Camera(cameraPosition); }
	~View() = default;

	// Methods
	bool createShaderProgram(const GLchar* const vertexShaderSource, const GLchar* const fragmentShaderSource);
	void destroyShaderProgram(GLuint programId) const;
	void resizeWindow(int width, int height) const;
	void switchViewType() { isPerspective = !isPerspective; }
	void setCameraSpeed(float speed);
	void drawShape(const Shape& shape) const;
	void setViewModePerspective() const;
	void setViewModeOrthographic() const;
	void useProgram(GLuint programId);

	// Setters
	void setProgramId(GLuint programId) { this->currentProgramId = programId; }
	void setCameraPosition(glm::vec3 position) { this->cameraPosition = position; }
	void setLastX(float x) { this->vLastX = x; }
	void setLastY(float y) { this->vLastY = y; }
	void setFirstMouse(bool isFirst) { this->vFirstMouse = isFirst; }
	void setWindow(GLFWwindow* window) { this->vWindow = window; }

	// Getters
	GLuint getProgramId() const { return currentProgramId; }
	Camera& getCamera() { return vCamera; }
	glm::vec3 getCameraPosition() const { return cameraPosition; }
	float getLastX() const { return vLastX; }
	float getLastY() const { return vLastY; }
	bool getFirstMouse() const { return vFirstMouse; }
	GLFWwindow* getWindow() const { return vWindow; }
	bool isViewPerspective() const { return isPerspective; }

	// Wrappers
	static void resizeWindowWrapper(GLFWwindow* window, int width, int height);

	// Attributes
	const char* const WINDOW_TITLE = "Timothy Genz";
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

private:
	Camera vCamera;
	GLuint currentProgramId{};

	glm::vec3 cameraPosition = glm::vec3(0.0f, 2.0f, 10.0f);
	float vLastX = (float)WINDOW_WIDTH / 2.0f;
	float vLastY = (float)WINDOW_HEIGHT / 2.0f;
	bool vFirstMouse = true;

	GLFWwindow* vWindow = nullptr;

	bool isPerspective = true;	// For toggling between perspective and orthographic view modes
};

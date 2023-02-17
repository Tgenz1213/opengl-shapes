#pragma once

#include "framework.h"
#include "Shape.h"
#include "View.h"

class Controller
{
public:
	// Constructors and deconstructor
	Controller() = default;
	explicit Controller(View* view);
	virtual ~Controller() = default;

	// Methods
	void run(int argc, char* argv[]);
	bool initialize(int argc, char* argv[]);

	// Attributes
	std::vector<Shape*> cShapes;
	View* cView;
	float cDeltaTime = 0.0f; // time between current frame and last frame
	float cLastFrame = 0.0f;

	// Methods
	void processInput();

private:
	// Callbacks
	void viewModeToggleCallback(GLFWwindow*, int key, int scancode, int action, int mods);
	void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
	void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	// Wrappers
	static void viewModeToggleWrapper(GLFWwindow*, int key, int scancode, int action, int mods);
	static void mousePositionWrapper(GLFWwindow* window, double xpos, double ypos);
	static void mouseScrollWrapper(GLFWwindow* window, double xoffset, double yoffset);

	// Debugging
	void foundGlError(const char* line);
};

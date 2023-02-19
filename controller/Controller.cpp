#include "Controller.h"

bool Controller::initialize(int argc, char* argv[])
{
	// GLFW: initialize and configure
// ------------------------------
	if (!glfwInit())
	{
		std::cerr << "Failed to initalize GLFW." << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// GLFW: window creation
	// ---------------------
	cView->setWindow(glfwCreateWindow(cView->WINDOW_WIDTH, cView->WINDOW_HEIGHT, cView->WINDOW_TITLE, nullptr, nullptr));
	if (cView->getWindow() == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Set the window user pointer to the View instance
	glfwSetWindowUserPointer(cView->getWindow(), this);

	// Context
	glfwMakeContextCurrent(cView->getWindow());

	// Callbacks
	glfwSetFramebufferSizeCallback(cView->getWindow(), View::resizeWindowWrapper);
	glfwSetCursorPosCallback(cView->getWindow(), mousePositionWrapper);
	glfwSetScrollCallback(cView->getWindow(), mouseScrollWrapper);
	glfwSetKeyCallback(cView->getWindow(), viewModeToggleWrapper);

	// tell GLFW to capture our mouse
	glfwSetInputMode(cView->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLEW: initialize
	// ----------------
	// Note: if using GLEW version 1.13 or earlier
	glewExperimental = GL_TRUE;

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW." << std::endl;
		return false;
	}

	// Displays GPU OpenGL version
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	return true;
}

void Controller::processInput()
{
	Camera& camera = cView->getCamera();

	// Exit
	if (glfwGetKey(cView->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(cView->getWindow(), true);

	// Built-in Camera methods for movement
	if (glfwGetKey(cView->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, cDeltaTime);
	if (glfwGetKey(cView->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, cDeltaTime);
	if (glfwGetKey(cView->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, cDeltaTime);
	if (glfwGetKey(cView->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, cDeltaTime);

	// Up and down movement
	if (glfwGetKey(cView->getWindow(), GLFW_KEY_E) == GLFW_PRESS)
		camera.Position += cView->getCamera().Up * cView->getCamera().MovementSpeed * cDeltaTime;
	if (glfwGetKey(cView->getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
		camera.Position -= cView->getCamera().Up * cView->getCamera().MovementSpeed * cDeltaTime;
}

void Controller::viewModeToggleCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		cView->switchViewType();	// Flip value
}

void Controller::mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	Camera& camera = cView->getCamera();

	if (cView->getFirstMouse())
	{
		cView->setLastX((float)xpos);
		cView->setLastY((float)ypos);
		cView->setFirstMouse(false);
	}

	float xoffset = (float)xpos - cView->getLastX();
	float yoffset = cView->getLastY() - (float)ypos; // reversed since y-coordinates go from bottom to top

	cView->setLastX((float)xpos);
	cView->setLastY((float)ypos);

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void Controller::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	cView->setCameraSpeed((GLfloat)yoffset);
}

void Controller::viewModeToggleWrapper(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	auto* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
	controller->viewModeToggleCallback(window, key, scancode, action, mods);
}

void Controller::mousePositionWrapper(GLFWwindow* window, double xpos, double ypos)
{
	auto* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
	controller->mousePositionCallback(window, xpos, ypos);
}

void Controller::mouseScrollWrapper(GLFWwindow* window, double xoffset, double yoffset)
{
	auto* controller = static_cast<Controller*>(glfwGetWindowUserPointer(window));
	controller->mouseScrollCallback(window, xoffset, yoffset);
}
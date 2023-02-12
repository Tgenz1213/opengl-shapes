#include "Controller.h"

Controller::Controller() : cView() {}

Controller::Controller(View* view) : cView(view) {}

// Render shapes. Must call Controller::initialize and View::createShaderProgram first
void Controller::run(int argc, char* argv[])
{
}

void Controller::foundGlError(const char* line)
{
	GLenum error;
	if ((error = glGetError()) != GL_NO_ERROR)
	{
		std::cout << line << " - OpenGL error: " << error << std::endl;
	}
}

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
	cView->vWindow = glfwCreateWindow(cView->WINDOW_WIDTH, cView->WINDOW_HEIGHT, cView->WINDOW_TITLE, nullptr, nullptr);
	if (cView->vWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Set the window user pointer to the View instance
	glfwSetWindowUserPointer(cView->vWindow, this);

	// Context
	glfwMakeContextCurrent(cView->vWindow);

	// Callbacks
	glfwSetFramebufferSizeCallback(cView->vWindow, View::resizeWindowWrapper);
	glfwSetCursorPosCallback(cView->vWindow, mousePositionWrapper);
	glfwSetScrollCallback(cView->vWindow, mouseScrollWrapper);
	glfwSetKeyCallback(cView->vWindow, viewModeToggleWrapper);

	// tell GLFW to capture our mouse
	glfwSetInputMode(cView->vWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

void Controller::addShape(Shape* shape)
{
	cShapes.push_back(shape);
}

void Controller::processInput()
{
	// Exit
	if (glfwGetKey(cView->vWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(cView->vWindow, true);

	// Built-in Camera methods for movement
	if (glfwGetKey(cView->vWindow, GLFW_KEY_W) == GLFW_PRESS)
		cView->vCamera.ProcessKeyboard(FORWARD, cDeltaTime);
	if (glfwGetKey(cView->vWindow, GLFW_KEY_S) == GLFW_PRESS)
		cView->vCamera.ProcessKeyboard(BACKWARD, cDeltaTime);
	if (glfwGetKey(cView->vWindow, GLFW_KEY_A) == GLFW_PRESS)
		cView->vCamera.ProcessKeyboard(LEFT, cDeltaTime);
	if (glfwGetKey(cView->vWindow, GLFW_KEY_D) == GLFW_PRESS)
		cView->vCamera.ProcessKeyboard(RIGHT, cDeltaTime);

	// Up and down movement
	if (glfwGetKey(cView->vWindow, GLFW_KEY_E) == GLFW_PRESS)
		cView->vCamera.Position += cView->vCamera.Up * cView->vCamera.MovementSpeed * cDeltaTime;
	if (glfwGetKey(cView->vWindow, GLFW_KEY_Q) == GLFW_PRESS)
		cView->vCamera.Position -= cView->vCamera.Up * cView->vCamera.MovementSpeed * cDeltaTime;
}

void Controller::setViewModePerspective()
{
	// Transforms the camera: move the camera back (z axis)
	glm::mat4 view = cView->vCamera.GetViewMatrix();

	glm::mat4 projection = glm::perspective(glm::radians(cView->vCamera.Zoom), (GLfloat)cView->WINDOW_WIDTH / (GLfloat)cView->WINDOW_HEIGHT, 0.1f, 100.0f);

	// Retrieves and passes transform matrices to the Shader program
	GLint viewLoc = glGetUniformLocation(cView->vProgramId, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLint projLoc = glGetUniformLocation(cView->vProgramId, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Controller::setViewModeOrthographic()
{
	// Transforms the camera: move the camera back (z axis)
	glm::mat4 view = cView->vCamera.GetViewMatrix();

	glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

	// Retrieves and passes transform matrices to the Shader program
	GLint viewLoc = glGetUniformLocation(cView->vProgramId, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLint projLoc = glGetUniformLocation(cView->vProgramId, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Controller::viewModeToggleCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		cView->isOrtho = !cView->isOrtho;	// Flip value
}

void Controller::mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (cView->vFirstMouse)
	{
		cView->vLastX = (float)xpos;
		cView->vLastY = (float)ypos;
		cView->vFirstMouse = false;
	}

	float xoffset = (float)xpos - cView->vLastX;
	float yoffset = cView->vLastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	cView->vLastX = (float)xpos;
	cView->vLastY = (float)ypos;

	cView->vCamera.ProcessMouseMovement(xoffset, yoffset);
}

void Controller::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	static const float cameraSpeed = 2.5f;

	cView->vCamera.MovementSpeed += cameraSpeed * (float)yoffset;
	if (cView->vCamera.MovementSpeed < cameraSpeed)
		cView->vCamera.MovementSpeed = cameraSpeed;
	if (cView->vCamera.MovementSpeed > cameraSpeed * 3.0f)
		cView->vCamera.MovementSpeed = cameraSpeed * 3.0f;
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
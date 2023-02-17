#include "Source.h"

int main(int argc, char* argv[])
{
	// Method variables
	GLuint shapeProgramId{};
	GLuint lightProgramId{};
	GLuint woodTexId{};
	GLuint orangeTexId{};
	GLuint tileTexId{};

	// Initialize view and factory
	View mView;
	ShapeFactory mFactory;
	const char* textureFilenameWood = "C:/Users/Tim/source/repos/cs330/resources/woodgrain.jpg";
	const char* textureFilenameOrange = "C:/Users/Tim/source/repos/cs330/resources/orange.jpg";
	const char* textureFilenameTile = "C:/Users/Tim/source/repos/cs330/resources/tile.jpg";

	// Initialize controller
	Controller mController(&mView);

	if (!mController.initialize(argc, argv))
		return EXIT_FAILURE;

	// Create shapes for tape measure
	Cube mCubeSmall = mFactory.createCube();
	Cube mCubeLarge = mFactory.createCube();
	UCylinder mCylinder = mFactory.createCylinder();
	Plane mPlane = mFactory.createPlane();

	// Lights
	Cube mLightCubeKey = mFactory.createCube();
	Cube mLightCubeFill = mFactory.createCube();

	// Create the shader programs
	if (!mView.createShaderProgram(ShaderNormalsTex::vertexShaderSource, ShaderNormalsTex::fragmentShaderSource))
		return EXIT_FAILURE;

	shapeProgramId = mView.getProgramId();

	if (!mView.createShaderProgram(ShaderLight::vertexShaderSource, ShaderLight::fragmentShaderSource))
		return EXIT_FAILURE;

	// Load textures
	lightProgramId = mView.getProgramId();

	mView.useProgram(shapeProgramId);

	// Wood texture
	if (!ULoadTexture(textureFilenameWood, woodTexId))
	{
		std::cout << "Failed to load cube texture " << woodTexId << std::endl;
		return EXIT_FAILURE;
	}

	glUniform1i(glGetUniformLocation(mView.getProgramId(), "uTexture"), 0);

	// Orange texture
	if (!ULoadTexture(textureFilenameOrange, orangeTexId))
	{
		std::cout << "Failed to load cylinder texture " << orangeTexId << std::endl;
		return EXIT_FAILURE;
	}

	glUniform1i(glGetUniformLocation(mView.getProgramId(), "uTexture"), 1);

	// Tile texture
	if (!ULoadTexture(textureFilenameTile, tileTexId))
	{
		std::cout << "Failed to load cylinder texture " << tileTexId << std::endl;
		return EXIT_FAILURE;
	}

	glUniform1i(glGetUniformLocation(mView.getProgramId(), "uTexture"), 2);

	// Associate shapes with textures
	mCubeLarge.texId = woodTexId;
	mCubeSmall.texId = woodTexId;
	mCylinder.texId = orangeTexId;
	mPlane.texId = tileTexId;

	// Set background color to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// render loop
	// -----------
	// Main loop
	while (!glfwWindowShouldClose(mView.getWindow()))
	{
		// per-frame timing
		// --------------------
		auto currentFrame = static_cast<float>(glfwGetTime());
		mController.cDeltaTime = currentFrame - mController.cLastFrame;
		mController.cLastFrame = currentFrame;

		mView.useProgram(shapeProgramId);

		// input
		// -----
		mController.processInput();

		// Enable z-depth
		glEnable(GL_DEPTH_TEST);

		// Clear the color buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Render shapes

		// Commented out the tape measure shapes for assignment 6-3

		// Draw cubes
		UTranslateShape(&mCubeSmall, glm::vec3(1.0f, 1.0f, 0.75f), glm::vec3(-0.3f, -0.5f, 0.0f), 0, &mView); // Retrieves and passes transform matrices to the Shader program
		mView.drawShape(mCubeSmall);

		UTranslateShape(&mCubeLarge, glm::vec3(1.0f, 1.0f, 0.99f), glm::vec3(0.5f, -0.5f, 0.0f), 0, &mView);
		mView.drawShape(mCubeLarge);

		// Draw cylinder
		UTranslateShape(&mCylinder, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0, &mView);
		mView.drawShape(mCylinder);

		// Draw cylinder
		UTranslateShape(&mPlane, glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, -0.501f, 0.0f), 0, &mView);
		mView.drawShape(mPlane);

		// Set view mode. Variable changed with P key press
		mView.setViewModePerspective();

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Render lights

		// Key light data
		glm::vec3 keyLightColor(1.0f, 1.0f, 1.0f);
		glm::vec3 keyLightPosition(7.0f, 0.5f, 7.0f);
		glm::vec3 keyLightScale(0.3f);

		// Fill light data
		glm::vec3 fillLightColor(1.0f, 1.0f, 1.0f);		// 10% intensity
		glm::vec3 fillLightPosition(-7.0f, 0.5f, -7.0f);	// Opposite side of key light
		glm::vec3 fillLightScale(0.3f);

		// Reference matrix uniforms from the Shape Shader program for the light color, light position, and camera position
		GLint keyLightColorLoc = glGetUniformLocation(mView.getProgramId(), "keyLightColor");
		GLint keyLightPositionLoc = glGetUniformLocation(mView.getProgramId(), "keyLightPos");
		GLint viewPositionLoc = glGetUniformLocation(mView.getProgramId(), "viewPosition");

		// Pass color, light, and camera data to the Shape Shader program's corresponding uniforms
		glUniform3f(keyLightColorLoc, keyLightColor.r, keyLightColor.g, keyLightColor.b);
		glUniform3f(keyLightPositionLoc, keyLightPosition.x, keyLightPosition.y, keyLightPosition.z);
		const glm::vec3 cameraPosition = mView.getCamera().Position;
		glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		GLint fillLightColorLoc = glGetUniformLocation(mView.getProgramId(), "fillLightColor");
		GLint fillLightPositionLoc = glGetUniformLocation(mView.getProgramId(), "fillLightPos");

		// Pass color, light, and camera data to the Shape Shader program's corresponding uniforms
		glUniform3f(fillLightColorLoc, fillLightColor.r, fillLightColor.g, fillLightColor.b);
		glUniform3f(fillLightPositionLoc, fillLightPosition.x, fillLightPosition.y, fillLightPosition.z);

		mView.useProgram(lightProgramId);

		// Key light
		UTranslateShape(&mLightCubeKey, keyLightScale, keyLightPosition, 0.0f, &mView);

		mView.setViewModePerspective();

		// Draw key light
		glBindVertexArray(mLightCubeKey.vao);
		mLightCubeKey.draw();
		glBindVertexArray(0);

		//Transform the smaller cube used as a visual que for the light source
		UTranslateShape(&mLightCubeFill, fillLightScale, fillLightPosition, 0.0f, &mView);

		mView.setViewModePerspective();

		// Draw key light
		glBindVertexArray(mLightCubeFill.vao);
		mLightCubeFill.draw();
		glBindVertexArray(0);
		// End fill light

		// Swap buffers
		glfwSwapBuffers(mView.getWindow());

		// Handle input events
		glfwPollEvents();
	}

	// Release textures
	glGenTextures(1, &woodTexId);
	glGenTextures(1, &orangeTexId);

	// Release shader program
	mView.destroyShaderProgram(shapeProgramId);
	mView.destroyShaderProgram(lightProgramId);

	exit(EXIT_SUCCESS); // Terminates the program successfully
}

// Add texture to shape
bool ULoadTexture(const char* imagePath, GLuint& texId)
{
	int width;
	int height;
	int channels;
	unsigned char* image = stbi_load(imagePath, &width, &height, &channels, 0);
	if (image)
	{
		UFlipImageVertically(image, width, height, channels);

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);

		// Set the texture wrapping parameters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << channels << " channels" << std::endl;
			return false;
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture.

		return true;
	}

	// Error loading the image
	return false;
}

// Flip image to match OpenGL Y-coordinates
void UFlipImageVertically(unsigned char* image, int width, int height, int channels)
{
	for (int j = 0; j < height / 2; ++j)
	{
		int index1 = j * width * channels;
		int index2 = (height - 1 - j) * width * channels;

		if (index1 >= height * width * channels || index2 >= height * width * channels)
		{
			std::cout << "Image flip failed. Width, height, or channels are incorrect for " << image << std::endl;
			break;
		}

		for (int i = width * channels; i > 0; --i)
		{
			unsigned char tmp = image[index1];
			image[index1] = image[index2];
			image[index2] = tmp;
			++index1;
			++index2;
		}
	}
}

// Modifies a shape to rotate, scale, and translate
void UTranslateShape(Shape* shape, glm::vec3 scale, glm::vec3 translation, float rotation, const View* view)
{
	// 1. Scales the object
	glm::mat4 scaleMatrix = glm::scale(scale);
	// 2. Rotates shape in the x axis
	glm::mat4 rotationMatrix = glm::rotate(rotation, glm::vec3(1.0f, 1.0f, 1.0f));
	// 3. Place object from the origin
	glm::mat4 translationMatrix = glm::translate(translation);
	// Model matrix: transformations are applied right-to-left order
	shape->model = scaleMatrix * rotationMatrix * translationMatrix;

	// Retrieves and passes transform matrices to the Shader program
	GLint modelLoc = glGetUniformLocation(view->getProgramId(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(shape->model));
}
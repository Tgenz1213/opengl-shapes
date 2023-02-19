#include "Source.h"

int main(int argc, char* argv[])
{
	// Method variables
	GLuint shapeProgramId{};
	GLuint lightProgramId{};

	// Initialize classes
	View mView;
	ShapeFactory mFactory;
	Controller mController(&mView);

	if (!mController.initialize(argc, argv))
		return EXIT_FAILURE;

	// Complex objects
	ComplexShape::TapeMeasure mTapeMeasure;
	ComplexShape::WhiskeyBottle mBottle;
	ComplexShape::Floor mFloor;

	// Lights, primitive shapes with no texture
	Cube mLightCubeKey = mFactory.createCube();
	Cube mLightCubeFill = mFactory.createCube();

	// Create the shader programs
	if (!mView.createShaderProgram(ShaderNormalsTex::vertexShaderSource, ShaderNormalsTex::fragmentShaderSource))
		return EXIT_FAILURE;

	shapeProgramId = mView.getProgramId();

	if (!mView.createShaderProgram(ShaderLight::vertexShaderSource, ShaderLight::fragmentShaderSource))
		return EXIT_FAILURE;

	lightProgramId = mView.getProgramId();

	// Set up complex shapes
	mView.useProgram(shapeProgramId);

	if (!UInitializeTapeMeasure(mView, mFactory, mTapeMeasure))
	{
		std::cout << "Failed to initialize tape measure" << std::endl;
		return EXIT_FAILURE;
	}

	if (!UInitializeWhiskeyBottle(mView, mFactory, mBottle))
	{
		std::cout << "Failed to initialize whiskey bottle" << std::endl;
		return EXIT_FAILURE;
	}

	// Tile texture
	if (!UInitializeFloor(mView, mFactory, mFloor))
	{
		std::cout << "Failed to initialize floor" << std::endl;
		return EXIT_FAILURE;
	}

	// Associate shapes with textures

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
		UDrawTapeMeasure(mView, mTapeMeasure);
		UDrawWhiskeyBottle(mView, mBottle);
		UDrawFloor(mView, mFloor);

		// Set view mode. Variable changed with P key press
		mView.setViewModePerspective();

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Render lighting

		// Key light data
		glm::vec3 keyLightColor(1.0f, 1.0f, 1.0f);	// White
		glm::vec3 keyLightPosition(7.0f, 2.0f, 7.0f);
		glm::vec3 keyLightScale(0.3f);

		// Fill light data
		glm::vec3 fillLightColor(0.0f, 0.3f, 0.3f);		// Greenish
		glm::vec3 fillLightPosition(-5.0f, 3.0f, -5.0f);	// Opposite side of key light, little higher
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
		UTranslateShape(mView, keyLightScale, keyLightPosition, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, &mLightCubeKey);
		glBindVertexArray(mLightCubeKey.vao);
		mLightCubeKey.draw();
		glBindVertexArray(0);

		//Transform the smaller cube used as a visual que for the light source
		UTranslateShape(mView, fillLightScale, fillLightPosition, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, &mLightCubeFill);
		glBindVertexArray(mLightCubeFill.vao);
		mLightCubeFill.draw();
		glBindVertexArray(0);

		mView.setViewModePerspective();

		// Swap buffers
		glfwSwapBuffers(mView.getWindow());

		// Handle input events
		glfwPollEvents();
	}

	// Release textures
	glGenTextures(1, &mTapeMeasure.woodTexId);
	glGenTextures(1, &mTapeMeasure.orangeTexId);
	glGenTextures(1, &mFloor.tileTexId);

	// Release shader program
	mView.destroyShaderProgram(shapeProgramId);
	mView.destroyShaderProgram(lightProgramId);

	exit(EXIT_SUCCESS); // Terminates the program successfully
}

bool ULoadTexture(const View& view, const char* imagePath, GLuint& texId)
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

		glUniform1i(glGetUniformLocation(view.getProgramId(), "uTexture"), texId);

		return true;
	}

	// Error loading the image
	std::cout << "Failed to load texture " << texId << std::endl;
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

bool UInitializeTapeMeasure(const View& view, const ShapeFactory& factory, ComplexShape::TapeMeasure& tapeMeasure)
{
	// Create shapes for tape measure
	tapeMeasure.largeCube = factory.createCube();
	tapeMeasure.smallCube = factory.createCube();
	tapeMeasure.cylinder = factory.createCylinder(1.0f, 1.0f, 1.0f, 36, 3);

	// Load wood texture
	if (!ULoadTexture(view, tapeMeasure.textureFilenameWood, tapeMeasure.woodTexId))
		return false;

	// Load orange texture
	if (!ULoadTexture(view, tapeMeasure.textureFilenameOrange, tapeMeasure.orangeTexId))
		return false;

	// Assign texture IDs to objects
	tapeMeasure.largeCube.texId = tapeMeasure.woodTexId;
	tapeMeasure.smallCube.texId = tapeMeasure.woodTexId;
	tapeMeasure.cylinder.texId = tapeMeasure.orangeTexId;

	return true;
}

bool UInitializeWhiskeyBottle(const View& view, const ShapeFactory& factory, ComplexShape::WhiskeyBottle& bottle)
{
	bottle.cap = factory.createCylinder(0.2f, 0.2f, 0.2f, 32, 3);
	bottle.neck = factory.createCylinder(0.4f, 0.2f, 1.0f, 32, 4);
	bottle.neckBase = factory.createSphere(0.999f, 16, 16);
	bottle.body = factory.createCylinder(1.0f, 1.0f, 3.0f, 32, 4);
	bottle.bodyBase = factory.createTorus(0.9f, 0.1f, 32);

	if (!ULoadTexture(view, bottle.textureFilenameIce, bottle.iceTexId))
		return false;

	if (!ULoadTexture(view, bottle.textureFilenameGold, bottle.goldTexId))
		return false;

	bottle.bodyBase.texId = bottle.iceTexId;
	bottle.body.texId = bottle.goldTexId;
	bottle.neckBase.texId = bottle.iceTexId;
	bottle.neck.texId = bottle.iceTexId;
	bottle.cap.texId = bottle.goldTexId;

	return true;
}

bool UInitializeFloor(const View& view, const ShapeFactory& factory, ComplexShape::Floor& floor)
{
	floor.plane = factory.createPlane();

	if (!ULoadTexture(view, floor.textureFilenameTile, floor.tileTexId))
		return false;

	floor.plane.texId = floor.tileTexId;

	return true;
}

void UDrawTapeMeasure(const View& view, ComplexShape::TapeMeasure& tapeMeasure)
{
	// Draw tape measure
	glm::vec3 tapeMeasureTranslation(-1.0f, 0.0f, 0.0f); // complex shape translation
	glm::vec3 smallCubeTranslation(-0.3f, 0.0f, 0.5f); // smallCube translation
	glm::vec3 largeCubeTranslation(0.5f, 0.0f, 0.5001f); // largeCube translation

	// Draw cubes
	UTranslateShape(view, glm::vec3(1.0f, 0.75f, 1.0f), smallCubeTranslation, tapeMeasureTranslation, 0.0f, &tapeMeasure.smallCube);
	view.drawShape(tapeMeasure.smallCube);

	UTranslateShape(view, glm::vec3(1.0f, 0.99f, 1.0f), largeCubeTranslation, tapeMeasureTranslation, 0.0f, &tapeMeasure.largeCube);
	view.drawShape(tapeMeasure.largeCube);

	// Draw cylinder

	UTranslateShape(view, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), tapeMeasureTranslation, 0.0f, &tapeMeasure.cylinder);
	view.drawShape(tapeMeasure.cylinder);
}

void UDrawWhiskeyBottle(const View& view, ComplexShape::WhiskeyBottle& bottle)
{
	glm::vec3 bottleTranslation(1.5f, 0.0f, 0.0f);

	glm::vec3 bodyBaseTranslation(0.0f, -0.4f, 0.0f);
	glm::vec3 bodyTranslation(0.0f, 1.1f, 0.0f);
	glm::vec3 neckBaseTranslation(0.0f, 2.5f, 0.0f);
	glm::vec3 neckTranslation(0.0f, 3.6f, 0.0f);
	glm::vec3 capTranslation(0.0f, 4.2f, 0.0f);

	// Bottom to top
	UTranslateShape(view, glm::vec3(1.0f, 1.0f, 1.0f), bodyBaseTranslation, bottleTranslation, 0.0f, &bottle.bodyBase);
	view.drawShape(bottle.bodyBase);

	UTranslateShape(view, glm::vec3(1.0f, 1.0f, 1.0f), bodyTranslation, bottleTranslation, 0.0f, &bottle.body);
	view.drawShape(bottle.body);

	UTranslateShape(view, glm::vec3(1.0f, 1.0f, 1.0f), neckBaseTranslation, bottleTranslation, 0.0f, &bottle.neckBase);
	view.drawShape(bottle.neckBase);

	UTranslateShape(view, glm::vec3(1.0f, 1.0f, 1.0f), neckTranslation, bottleTranslation, 0.0f, &bottle.neck);
	view.drawShape(bottle.neck);

	UTranslateShape(view, glm::vec3(1.0f, 1.0f, 1.0f), capTranslation, bottleTranslation, 0.0f, &bottle.cap);
	view.drawShape(bottle.cap);
}

void UDrawFloor(const View& view, ComplexShape::Floor& floor)
{
	glm::vec3 floorTranslation(0.0f, -0.501f, 0.0f);

	UTranslateShape(view, glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), floorTranslation, 0.0f, &floor.plane);
	view.drawShape(floor.plane);
}

// Modifies primitive shape to rotate, scale, and translate and translates shapes part of a complex translation equally.
void UTranslateShape(const View& view, const glm::vec3 scale, const glm::vec3 primitiveTranslation, const glm::vec3 complexTranslation, const float rotation, Shape* shape)
{
	// 1. Place object from the origin. PrimitiveTranslation moves the individual shape to form the
	// complex shape. ComplexTranslation moves all of the primitives together.
	glm::mat4 translationMatrix = glm::translate(primitiveTranslation + complexTranslation);
	// 2. Scales the object
	glm::mat4 scaleMatrix = glm::scale(scale);
	// 3. Rotates shape in the x axis
	glm::mat4 rotationMatrix = glm::rotate(rotation, glm::vec3(1.0f, 1.0f, 1.0f));

	// Model matrix: transformations are applied right-to-left order
	shape->model = translationMatrix * scaleMatrix * rotationMatrix;

	// Retrieves and passes transform matrices to the Shader program
	GLint modelLoc = glGetUniformLocation(view.getProgramId(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(shape->model));
}
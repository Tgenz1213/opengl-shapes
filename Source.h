#pragma once

#include "framework.h"
#include "Controller.h"
#include "View.h"
#include "ShapeFactory.h"
#include "Shaders.h"
// Image loader
#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

// Complex shapes that use at least one primitive shape and texture. There are three complex shapes
// that use five different primitive shapes.
namespace ComplexShape
{
	// Uses two different primitive shapes
	struct TapeMeasure
	{
		// Primitive shapes
		Cube largeCube;
		Cube smallCube;
		Cylinder cylinder;

		// Texture ID's used
		GLuint woodTexId{};
		GLuint orangeTexId{};

		// Texture file locations
		const char* textureFilenameOrange = "C:/Users/Tim/source/repos/cs330/resources/orange.jpg";
		const char* textureFilenameWood = "C:/Users/Tim/source/repos/cs330/resources/woodgrain.jpg";
	};

	// Uses three different primitive shapes
	struct WhiskeyBottle
	{
		// Primitive shapes
		Cylinder body;
		Cylinder cap;
		Cylinder neck;
		Sphere neckBase;
		Torus bodyBase;

		// Texture ID's used
		GLuint iceTexId{};
		GLuint goldTexId{};

		// File locations
		const char* textureFilenameIce = "C:/Users/Tim/source/repos/cs330/resources/ice.jpg";
		const char* textureFilenameGold = "C:/Users/Tim/source/repos/cs330/resources/gold.jpg";
	};

	// Simple plane
	struct Floor
	{
		// Primitive shape
		Plane plane;

		// Texture ID
		GLuint tileTexId{};

		// File location
		const char* textureFilenameTile = "C:/Users/Tim/source/repos/cs330/resources/tile.jpg";
	};
}

// Scale, translate, and rotate to orient a primitive shape within the complex shape
void UTranslateShape(const View& view, const glm::vec3 scale, const glm::vec3 primitiveTranslation, const glm::vec3 complexTranslation, const float rotation, Shape* shape);

// Load texture
bool ULoadTexture(const View& view, const char* imagePath, GLuint& texId);
void UFlipImageVertically(unsigned char* image, int width, int height, int channels);	// Used by ULoadTexture()

// Initialize complex shapes
bool UInitializeTapeMeasure(const View& view, const ShapeFactory& factory, ComplexShape::TapeMeasure& tapeMeasure);
bool UInitializeWhiskeyBottle(const View& view, const ShapeFactory& factory, ComplexShape::WhiskeyBottle& bottle);
bool UInitializeFloor(const View& view, const ShapeFactory& factory, ComplexShape::Floor& floor);

// Draw complex shapes
void UDrawTapeMeasure(const View& view, ComplexShape::TapeMeasure& tapeMeasure);
void UDrawWhiskeyBottle(const View& view, ComplexShape::WhiskeyBottle& bottle);
void UDrawFloor(const View& view, ComplexShape::Floor& floor);
#pragma once

#include "framework.h"
#include "Controller.h"
#include "View.h"
#include "ShapeFactory.h"
#include "Shaders.h"

// Image loader
#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

// Scale, translate, and rotate a shape
void UTranslateShape(Shape* shape, glm::vec3 scale, glm::vec3 translation, float rotation , const View* view);

// Load Texture
bool ULoadTexture(const char* imagePath, Shape* shape);
void UFlipImageVertically(unsigned char* image, int width, int height, int channels);
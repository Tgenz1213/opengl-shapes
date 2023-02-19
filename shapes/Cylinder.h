#pragma once
#include "Shape.h"

// This class uses Cylinder.cpp by Song Ho Ahn to create a Shape

class Cylinder : public Shape
{
public:
	// Constructor and deconstructor
	Cylinder() = default;
	Cylinder(GLfloat baseRadius, GLfloat topRadius, GLfloat height, GLuint nSides, GLuint nStacks);
	~Cylinder() final = default;

	// Inherited via Shape
	void createVertices() override;
	void createNormals() override;
	void createTextureCoordinates() override;
	void createIndices() override;

	// Setters
	void setBaseRadius(GLfloat const radius) { this->baseRadius = radius; }
	void setTopRadius(GLfloat const radius) { this->topRadius = radius; }
	void setHeight(GLfloat const ht) { this->height = ht; }
	void setSides(GLuint const sides) { this->nSides = sides; }
	void setStacks(GLuint const stacks) { this->nStacks = stacks; }

	// Getters
	GLfloat getBaseRadius() const { return baseRadius; }
	GLfloat getTopRadius() const { return topRadius; }
	GLfloat getHeight() const { return height; }
	GLuint getSides() const { return nSides; }
	GLuint getStack() const { return nStacks; }

private:
	GLfloat baseRadius{};
	GLfloat topRadius{};
	GLfloat height{};
	GLuint nSides{};
	GLuint nStacks{};
};

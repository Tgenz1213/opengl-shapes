#pragma once
#include "Shape.h"
class Sphere : public Shape
{
public:
	// Constructor and deconstructor
	Sphere() = default;
	Sphere(GLfloat nRadius, GLuint nSlices, GLuint nStacks);
	~Sphere() final = default;

	// Inherited via Shape
	void createVertices() override;
	void createNormals() override;
	void createTextureCoordinates() override;
	void createIndices() override;

	// Setters
	void setRadius(GLfloat nRadius) { this->radius = nRadius; }
	void setSlices(GLuint nSlices) { this->slices = nSlices; }
	void setStacks(GLuint nStacks) { this->stacks = nStacks; }

	// Getters
	GLfloat getRadius() const { return radius; }
	GLuint getSlices() const { return slices; }
	GLuint getStacks() const { return stacks; }
private:
	GLfloat radius{};
	GLuint slices{};
	GLuint stacks{};
};


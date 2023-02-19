#pragma once
#include "Shape.h"

// Primitive torus (donut with hole) shape. Must use constructor with arguments to finish initializing class.
class Torus : public Shape
{
public:
	// Constructor and deconstructor
	Torus() = default;
	Torus(GLfloat majorRad, GLfloat minorRad, GLuint nSides);
	~Torus() final = default;

	// Inherited via Shape
	void createVertices() override;
	void createNormals() override;
	void createTextureCoordinates() override;
	void createIndices() override;

	// Setters
	void setMajorRadius(GLfloat radius) { this->majorRadius = radius; }
	void setMinorRadius(GLfloat radius) { this->minorRadius = radius; }
	void setSides(GLuint nSides) { this->sides = nSides; }

	// Getters
	GLfloat getMajorRadius() const { return majorRadius; }
	GLfloat getMinorRadius() const { return minorRadius; }
	GLuint getSides() const { return sides; }
private:
	GLfloat majorRadius{};
	GLfloat minorRadius{};
	GLuint sides{};
};

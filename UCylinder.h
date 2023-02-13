#pragma once
#include "Shape.h"

// This class uses Cylinder.cpp by Song Ho Ahn to create a Shape

class UCylinder : public Shape
{
public:
	// Constructor and deconstructor
	UCylinder() { bindVertexArrayAndBuffers(); }
	~UCylinder() final = default;

	// Inherited via Shape
	void createVertices() override;
	void createNormals() override;
	void createTextureCoordinates() override;
	void createIndices() override;

	Cylinder cylinder = Cylinder(1.0f, 1.0f, 1.0f, 20, 1, true);
};

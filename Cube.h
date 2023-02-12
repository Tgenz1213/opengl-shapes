// Simple cube

#pragma once
#include "Shape.h"
class Cube : public Shape
{
public:
	// Constructor and deconstructor
	Cube() { bindVertexArrayAndBuffers(); }
	~Cube() final = default;

	// Inherited via Shape
	void createVertices() override;
	void createNormals() override;
	void createTextureCoordinates() override;
	void createIndices() override;
};

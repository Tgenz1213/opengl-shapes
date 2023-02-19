#pragma once
#include "Shape.h"
class Plane : public Shape
{
public:
	// Constructor and deconstructor
	Plane() { bindVertexArrayAndBuffers(); }
	~Plane() final = default;

	// Inherited via Shape
	void createVertices() override;
	void createNormals() override;
	void createTextureCoordinates() override;
	void createIndices() override;
};

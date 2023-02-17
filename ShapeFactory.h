#pragma once
#include "Shape.h"
#include "Cube.h"
#include "PyramidSqBase.h"
#include "UCylinder.h"
#include "Plane.h"

class ShapeFactory
{
public:
	// Constructor and deconstructor
	ShapeFactory() = default;
	~ShapeFactory() = default;

	// Factory methods
	Cube createCube() const { return Cube(); }
	PyramidSqBase createPyramid() const { return PyramidSqBase(); }
	UCylinder createCylinder() const { return UCylinder(); }
	Plane createPlane() const { return Plane(); }
};

#pragma once

#include "framework.h"

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

////////////////////////////////////////////////////////////////
// Render shapes that only have color coordinates
//
namespace ShaderColorOnly
{
	/* Vertex Shader Source Code*/
	const GLchar* const vertexShaderSource = GLSL(440,
		layout(location = 0) in vec3 position; // Vertex data from Vertex Attrib Pointer 0
	layout(location = 1) in vec4 color;  // Color data from Vertex Attrib Pointer 1

	out vec4 vertexColor; // variable to transfer color data to the fragment shader

	//Global variables for the  transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f); // transforms vertices to clip coordinates
		vertexColor = color; // references incoming color data
	}
	);

	/* Fragment Shader Source Code*/
	const GLchar* const fragmentShaderSource = GLSL(440,
		in vec4 vertexColor; // Variable to hold incoming color data from vertex shader

	out vec4 fragmentColor;

	void main()
	{
		fragmentColor = vec4(vertexColor);
	}
	);
}

////////////////////////////////////////////////////////////////
// Render shapes that have texture coordinates
// Must modify fragment shader to use different textures on different shapes
//
namespace ShaderTexOnly
{
	/* Vertex Shader Source Code */
	const GLchar* const vertexShaderSource = GLSL(440,
		layout(location = 0) in vec3 position;
	layout(location = 2) in vec2 texCoord;

	out vec2 vertexTextureCoordinate;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f);
		vertexTextureCoordinate = texCoord;
	}
	);

	/* Fragment Shader Source Code */
	const GLchar* const fragmentShaderSource = GLSL(440,
		in vec2 vertexTextureCoordinate;

	out vec4 fragmentColor;

	uniform sampler2D texture0;
	uniform sampler2D texture1;
	uniform int activeTexture;

	void main()
	{
		if (activeTexture == 0)
			fragmentColor = texture(texture0, vertexTextureCoordinate);
		else if (activeTexture == 1)
			fragmentColor = texture(texture1, vertexTextureCoordinate);
	}
	);
}

////////////////////////////////////////////////////////////////
// Render shapes that have normals and texture coordinates
//
namespace ShaderNormalsTex
{
	// TODO: implement shader source code
}

////////////////////////////////////////////////////////////////
// Render light sources
//
namespace ShaderLight
{
	// TODO: implement shader source code
}
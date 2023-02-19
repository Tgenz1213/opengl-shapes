#pragma once

#include "framework.h"

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

////////////////////////////////////////////////////////////////
// Render shapes that only have color coordinates
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
// Render shapes that have texture coordinates Must modify fragment shader to use different textures
// on different shapes
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
namespace ShaderNormalsTex
{
	/* Vertex Shader Source Code */
	const GLchar* const vertexShaderSource = GLSL(440,

		layout(location = 0) in vec3 position; // VAP position 0 for vertex position data
	layout(location = 1) in vec3 normal; // VAP position 1 for normals
	layout(location = 2) in vec2 textureCoordinate;

	out vec3 vertexNormal; // For outgoing normals to fragment shader
	out vec3 vertexFragmentPos; // For outgoing color / pixels to fragment shader
	out vec2 vertexTextureCoordinate;

	//Uniform / Global variables for the  transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates

		vertexFragmentPos = vec3(model * vec4(position, 1.0f)); // Gets fragment / pixel position in world space only (exclude view and projection)

		vertexNormal = mat3(transpose(inverse(model))) * normal; // get normal vectors in world space only and exclude normal translation properties
		vertexTextureCoordinate = textureCoordinate;
	}
	);

	/* Fragment Shader Source Code*/
	const GLchar* const fragmentShaderSource = GLSL(440,

		in vec3 vertexNormal; // For incoming normals
	in vec3 vertexFragmentPos; // For incoming fragment position
	in vec2 vertexTextureCoordinate;

	out vec4 fragmentColor; // For outgoing cube color to the GPU

	// Uniform / Global variables for object color, light color, light position, and camera/view position
	uniform vec3 keyLightColor;
	uniform vec3 keyLightPos;
	uniform vec3 fillLightColor;
	uniform vec3 fillLightPos;
	uniform vec3 viewPosition;
	uniform sampler2D uTexture; // Useful when working with multiple textures

	void main()
	{
		/*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

		/* Calculate Ambient lighting */
		float ambientStrength = 0.1f; // Set ambient or global lighting strength
		vec3 ambient = ambientStrength * (keyLightColor + fillLightColor); // Generate ambient light color

		/* Calculate Diffuse lighting for key light */
		vec3 norm = normalize(vertexNormal); // Normalize vectors to 1 unit
		vec3 keyLightDirection = normalize(keyLightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
		float impact = max(dot(norm, keyLightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
		vec3 diffuseKey = impact * keyLightColor; // Generate diffuse light color

		/* Calculate Diffuse lighting for fill light */
		vec3 fillLightDirection = normalize(fillLightPos - vertexFragmentPos); // Calculate distance (light direction) between light source and fragments/pixels on cube
		impact = max(dot(norm, fillLightDirection), 0.0);// Calculate diffuse impact by generating dot product of normal and light
		vec3 diffuseFill = impact * fillLightColor; // Generate diffuse light color

		/* Calculate Specular lighting for key light */
		float specularIntensity = 1.0f; // Set specular light strength to 100%
		float highlightSize = 16.0f; // Set specular highlight size
		vec3 viewDir = normalize(viewPosition - vertexFragmentPos); // Calculate view direction
		vec3 reflectDir = reflect(-keyLightDirection, norm);// Calculate reflection vector
		// Calculate specular component for key light
		float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
		vec3 specularKey = specularIntensity * specularComponent * keyLightColor;

		/* Calculate Specular light for fill light */
		specularIntensity = 1.0f;	// Set specular light strength to 100%
		reflectDir = reflect(-fillLightDirection, norm);// Calculate reflection vector
		// Calculate specular component for fill light
		specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
		vec3 specularFill = specularIntensity * specularComponent * fillLightColor;

		// Texture holds the color to be used for all three components
		vec4 textureColor = texture(uTexture, vertexTextureCoordinate);

		// Calculate phong result
		vec3 phong = (ambient + diffuseKey + specularKey + diffuseFill + specularFill) * textureColor.xyz;

		fragmentColor = vec4(phong, 1.0); // Send lighting results to GPU
	}
	);
}

////////////////////////////////////////////////////////////////
// Render light sources
namespace ShaderLight
{
	/* Lamp Shader Source Code*/
	const GLchar* const vertexShaderSource = GLSL(440,

		layout(location = 0) in vec3 position; // VAP position 0 for vertex position data

	//Uniform / Global variables for the  transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
	}
	);

	/* Fragment Shader Source Code*/
	const GLchar* const fragmentShaderSource = GLSL(440,

		out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

	// Uniforms for changing the color and intensity of the light
	//uniform vec3 lightColor;
	//uniform float lightIntensity;

	void main()
	{
		fragmentColor = vec4(1.0f);
	}
	);
}
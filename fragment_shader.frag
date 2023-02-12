/* Fragment Shader Source Code*/
const GLchar* const fragmentShaderSource = GLSL(440,
	in vec4 vertexColor; // Variable to hold incoming color data from vertex shader

out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(vertexColor);
}
);
#version 430

in vec3 normal;
in vec4 color;

out vec4 fragmentColor; 

void main()
{
	fragmentColor = color;
}

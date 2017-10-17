#version 430


uniform bool u_isWireframe;

in vec3 color;

out vec4 fragmentColor;


void main()
{
	if (!u_isWireframe) {
			fragmentColor = vec4(color, 0.25);
	}
	else {
		fragmentColor = vec4(color, 1.0);
	}
}

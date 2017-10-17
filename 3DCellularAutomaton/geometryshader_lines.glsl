#version 430

layout(lines) in;
layout(line_strip, max_vertices = 2) out;

in ivec3 cellPosition[];
in int instanceID[];

out vec4 color;

layout(std430, binding = 3) buffer ssbo
{
	int shallBeRendered[];
};

void main()
{
	if (shallBeRendered[instanceID[0]] != 0) {
		for (int i = 0; i < 2; i++)
		{
			gl_Position = gl_in[i].gl_Position;
			color = vec4(0.0, 0.0, 0.0, 1.0); // black wireframe cube
			EmitVertex();
		}


		EndPrimitive();
	}
}
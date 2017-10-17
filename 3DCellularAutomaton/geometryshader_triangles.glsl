#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in ivec3 cellPosition[];
in int instanceID[];

out vec4 color;

uniform int u_numStates;
uniform vec3 u_gradient[11];

layout(std430, binding = 3) buffer ssbo
{
	int cellState[];
};

void main()
{
	int state = cellState[instanceID[0]];
	if (state != 0) {
		for (int i = 0; i < 3; i++)
		{
			gl_Position = gl_in[i].gl_Position;
			
			int stateFactor = int((float(state) / float(u_numStates))*10);
			
			color = vec4(u_gradient[stateFactor], 1.0);

			EmitVertex();
		}


		EndPrimitive();
	}
}
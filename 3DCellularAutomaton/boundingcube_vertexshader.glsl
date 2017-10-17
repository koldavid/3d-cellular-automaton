#version 430

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;

uniform mat4 u_projectionM;
uniform mat4 u_worldM;
uniform mat4 u_viewM;
uniform int u_cubeDimension;
uniform bool u_isWireframe;
uniform vec4 u_cubeColor;
uniform vec4 u_wireframeColor;

out vec4 color;

void main()
{
	mat4 scaleM = mat4(1.0);
	scaleM[0][0] = u_cubeDimension * 1.25;
	scaleM[1][1] = u_cubeDimension * 1.25;
	scaleM[2][2] = u_cubeDimension * 1.25;
	scaleM[3][3] = 1.0;
	mat4 WVP = u_projectionM * u_viewM * u_worldM * scaleM;
	gl_Position = WVP * vec4(a_vertex, 1.0) ;

	if (!u_isWireframe) {
		color = u_cubeColor;
	}
	else {
		color = u_wireframeColor;
	}
}

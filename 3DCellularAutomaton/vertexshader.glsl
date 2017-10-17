#version 430

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec3 a_normal;

uniform mat4 u_projectionM;
uniform mat4 u_worldM;
uniform mat4 u_viewM;
uniform int u_cubeDimension;

out vec3 normal;
out ivec3 cellPosition;
out int instanceID;

const float gapSize = 0.5;

void main()
{
	mat4 translate = mat4(1.0f);

	float gapFactor = 2.0 + gapSize;

	cellPosition.x = (gl_InstanceID) / (u_cubeDimension*u_cubeDimension);
	cellPosition.y = (gl_InstanceID / u_cubeDimension) % u_cubeDimension;
	cellPosition.z = (gl_InstanceID) % u_cubeDimension;

	translate[3][0] = (cellPosition.x - u_cubeDimension/2.0) * gapFactor;
	translate[3][1] = (cellPosition.y - u_cubeDimension/2.0) * gapFactor;
	translate[3][2] = (cellPosition.z - u_cubeDimension/2.0) * gapFactor;

	translate[3][0] += gapFactor/2.0;
	translate[3][1] += gapFactor/2.0;
	translate[3][2] += gapFactor/2.0;
	
	mat4 WVP = u_projectionM * u_viewM * u_worldM * translate;
	gl_Position = WVP * vec4(a_vertex, 1.0);
	normal = a_normal;
	instanceID = gl_InstanceID;
}

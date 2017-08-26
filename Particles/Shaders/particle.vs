#version 430 

layout(location=0) in vec4 pos;
layout(location=1) in vec4 vel;

uniform mat4 model;
uniform mat4 view=mat4(1.0);
uniform mat4 projection;

out vec3 velocity ;

void main ()
{
	
gl_Position= projection*view*model*pos;
velocity =vel.xyz;
}
#version 430 

layout(location=0) in vec4 pos;
layout(location=1) in vec3 norm;
layout(location=2) in vec2 texCoord;

uniform mat4 model;
uniform mat4 view=mat4(1.0);
uniform mat4 projection;

out vec3 position;
out vec3 normal;
out vec2 uv;

void main ()
{
vec4 p =model*pos;	

gl_Position= projection*view*p;

normal= mat3(transpose(inverse(model)))*norm;
position =p.xyz;
uv=texCoord;
}
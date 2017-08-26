#version 430 

layout(location=0) in vec4 pos;
layout(location=1) in vec4 norm;
layout(location=2) in vec2 texCoord;

uniform mat4 model = mat4(1);
uniform mat4 view;
uniform mat4 projection;
uniform mat4 projectionView;

out vec3 position;
out vec3 normal;
out vec2 uv;

void main ()
{
vec4 p =model*pos;	

gl_Position= projection*view*model*p;

normal= mat3(transpose(inverse(model)))*norm.xyz;
position =p.xyz;
uv=texCoord;
}
#version 400 core

layout(location=0) in vec3 pos ;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projectionView=mat4(1.0f);

out vec2 o_uv;
out vec3 o_normal;
out vec3 o_vertCoord;

void main ()
{
vec4 toworldCoord= model *vec4(pos, 1.0);
gl_Position = projectionView*toworldCoord;
o_uv= vec2(uv.x,1-uv.y);
o_normal = mat3(transpose(inverse(model)))* normal;
o_vertCoord=toworldCoord.xyz;
} 
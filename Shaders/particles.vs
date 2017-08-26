#version 400 core 


layout(location=0) in vec3 pos ;
layout(location=0) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 o_normal;
out vec3 o_fragPos;
void main()
{
vec4 modelPos = model *vec4(pos , 1.0);
gl_Position =projection*view*modelPos;
o_normal= mat3(transpose(inverse(model)))*normal;
o_fragPos=modelPos.xyz;
}
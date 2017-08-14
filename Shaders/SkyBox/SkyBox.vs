#version 330 core 
layout(location=0) in vec3 pos;

uniform mat4 projection ;
uniform mat4 view;

out  vec3 uv;
void main ()
{
 vec4 p = projection * view * vec4(pos, 1.0);
 
 gl_Position = p.xyww;
 uv =pos;
}
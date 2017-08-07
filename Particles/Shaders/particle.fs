#version 430 

out vec4 color;

uniform vec4 Pcolor;
uniform vec3 BlackHolePos1;
uniform vec3 BlackHolePos2;
in vec3 velocity ;

void main ()
{
float len=length(velocity)/220;

color = vec4(1,1.2-len ,1-sin(len),1);
}
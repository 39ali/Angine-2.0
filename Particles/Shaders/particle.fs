#version 430 

out vec4 color;

uniform vec4 Pcolor;

in vec3 velocity ;

void main ()
{
float len=length(velocity);
len =len/(256)-1;

color = vec4(sin(len),1-cos(1-len),1-tan(len),1);
//color = vec4(sin(1-len),1-cos(1-len),1-tan(len),1);
}
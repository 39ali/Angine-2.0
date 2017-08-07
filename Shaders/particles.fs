#version 400 core 


out vec4 color ;

in vec3 o_normal;
in vec3 o_fragPos;
uniform vec3 lightPos;

void main ()
{
vec3 lightDir = normalize(lightPos-o_fragPos);
vec3 norm = normalize(o_normal);

float diff = max(dot(lightDir,norm),0.2);

vec3 diffuse = diff*vec3(1,0,0);
color =vec4(diffuse,1);
}
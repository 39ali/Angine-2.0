#version 400 core

out vec4 color ; 

in vec2 o_uv;
in vec3 o_normal;
in vec3 o_vertCoord;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

float specularStrength=32.0f;
 vec3 diffuse = vec3(0.8) ;
 vec3 specular = vec3(0.2);
 vec3 LightIntensity = vec3(1.0);

void main()
{
vec3 norm = normalize(o_normal);
vec3 toLight = normalize(lightPos-o_vertCoord);
float diff= max(dot(norm,toLight),0.0);

vec3 reflectedLight = reflect(-toLight,norm);
vec3 viewDir= normalize(cameraPos-o_vertCoord);
float spec = pow(max(dot(viewDir,reflectedLight),0.0f),specularStrength);

vec3 final = LightIntensity*(diff*diffuse + spec*specular)*vec3(texture(diffuseMap,o_uv));
color = vec4(final,1.0f); 
}
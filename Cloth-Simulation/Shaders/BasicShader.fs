#version 400 core

out vec4 color ; 

in vec2 o_uv;
in vec3 o_normal;
in vec3 o_vertCoord;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

float specularStrength=0.5f;

void main()
{
vec3 norm = normalize(o_normal);
vec3 toLight = normalize(lightPos-o_vertCoord);
float diff= max(dot(norm,toLight),0.2);

vec3 reflectedLight = reflect(-toLight,norm);
vec3 viewDir= normalize(-cameraPos-o_vertCoord);
float spec = pow(max(dot(viewDir,reflectedLight),0.0f),64);

vec3 diffuse = diff*vec3(texture(diffuseMap,o_uv));
vec3 specular = specularStrength*spec*vec3(1,1,1);
//*vec3(texture(specularMap,o_uv));
color = vec4(diffuse+specular,1.0f); //vec4(spec,spec,spec,1.0) ; //vec4(0.6,0.6,0.1,1);
}
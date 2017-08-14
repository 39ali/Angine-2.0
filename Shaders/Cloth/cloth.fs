#version 430 

out vec4 color;

in vec3 position;
in vec3 normal;
in vec2 uv;


uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform sampler2D diffuseMap;

float specularStrength=32.0f;
 vec3 diffuse = vec3(0.8) ;
 vec3 specular = vec3(0.2);
 vec3 LightIntensity = vec3(1.0);

void main ()
{
vec3 normal = normalize(normal);
 vec3 tolight = normalize(lightPos-position);
vec3 tocam = normalize(cameraPos- position);

 float diff= max(dot(normal , tolight),0.0);

vec3 reflectedlight = reflect(-tolight,normal);

float spec = pow(max(dot(reflectedlight,tocam),0.0),specularStrength);

vec3 final = LightIntensity*(diff*diffuse + spec*specular)*vec3(texture(diffuseMap,uv));


color = vec4(final , 1.0f);
}
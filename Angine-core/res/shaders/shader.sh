#shader vertex
#version 330
layout(location=0) in vec3 position;
layout(location=1) in vec2 uv;
layout(location=2) in vec3 normal;
uniform mat4 translate;
uniform mat4 worldTransform;
out vec4 positionOut;
out vec2 uvOut;
out vec3 normalOut;
void main() {
  gl_Position = translate* vec4(position, 1.0);
 
  positionOut = vec4(clamp(position,0.0,1.0),1.0);
  uvOut=uv;
  normalOut = (worldTransform*vec4(normal,0.0)).xyz;
}


#shader fragment
#version 330
out vec4 fragColor;
in vec4 positionOut;
in vec2 uvOut;
in vec3 normalOut;

struct DirectionalLight {
	vec3 color;
	float ambientInten;
	vec3 direction;
	float diffuseInten;
};

uniform DirectionalLight gDirectionalLight;
uniform sampler2D gsampler;

void main(){
vec4 ambient=vec4(gDirectionalLight.color* gDirectionalLight.ambientInten,1.0f);
float diffuse = max(0.0 ,dot(normalize(normalOut), - gDirectionalLight.direction));
vec4 diffuseColor = vec4(gDirectionalLight.color* diffuse*gDirectionalLight.diffuseInten,1.0f);
fragColor = texture2D(gsampler,uvOut.xy)*(ambient+diffuseColor);
}
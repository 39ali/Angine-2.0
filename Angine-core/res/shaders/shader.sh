#shader vertex
#version 330
layout(location=0) in vec3 position;
layout(location=1) in vec2 uv;
uniform float scale;
uniform mat4 translate;

out vec4 positionOut;
out vec2 uvOut;
void main() {
  gl_Position = translate* vec4(position, 1.0);
 
  positionOut = vec4(clamp(position,0.0,1.0),1.0);
  uvOut=uv;
}


#shader fragment
#version 330
out vec4 fragColor;
in vec4 positionOut;
in vec2 uvOut;

uniform sampler2D gsampler;
void main(){
fragColor = texture2D(gsampler,uvOut.xy);
}
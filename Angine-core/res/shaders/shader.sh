#shader vertex
#version 330
layout(location = 0) in vec3 position;
uniform float scale;
uniform mat4 translate;

out vec4 positionOut;
void main() {
  gl_Position = translate* vec4(position, 1.0);
 
  positionOut = vec4(clamp(position,0.0,1.0),1.0);
}


#shader fragment
#version 330
out vec4 fragColor;
in vec4 positionOut;
void main(){
fragColor = positionOut;
}
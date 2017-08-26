#version 430

//invocations per work group
layout( local_size_x = 1000 ) in;

uniform float Gravity = 1500.0;
uniform vec3 BlackHolePos ;
uniform float BlackHoleM= 10;



uniform float ParticleMass = 1;
uniform float ParticleInvMass = 1.0 /1.0f;
uniform float DeltaT = 0.0005;
uniform float MaxDist = 10.0;

layout(std430, binding=0) buffer Pos {
  vec4 Position[];
};
layout(std430, binding=1) buffer Vel {
  vec4 Velocity[];
};

void main() {
  uint idx = gl_GlobalInvocationID.x;

  vec3 p = Position[idx].xyz;

  // Force from black hole
  vec3 d = BlackHolePos - p;
  float dist = length(d);
  vec3 force = Gravity*((BlackHoleM)*(ParticleMass)/ dist*dist) * normalize(d);





    //  Euler integrator
    vec3 a = force * ParticleInvMass;

    Position[idx] = vec4(
        p + Velocity[idx].xyz * DeltaT + 0.5 * a * DeltaT * DeltaT, 1.0);
    Velocity[idx] = vec4( Velocity[idx].xyz + a * DeltaT, 0.0);

}

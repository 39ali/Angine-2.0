#version 430

layout( local_size_x = 10, local_size_y = 10 ) in;

uniform vec3 Gravity = vec3(0,-9.8,0);
uniform float ParticleMass =0.1;// 0.1;
uniform float ParticleInvMass = 1.0 / 0.1;
uniform float SpringK = 1000.0;//2000.0;
uniform float RestLengthHoriz;
uniform float RestLengthVert;
uniform float RestLengthDiag;
uniform float DeltaT = 0.000005;
uniform float DampingConst = 0.1;

uniform float SphereR;
uniform vec3 SpherePos;


uniform int selectedVert;
uniform vec3 mouseForce;

layout(std430, binding=0) buffer PosIn {
  vec4 PositionIn[];
};
layout(std430, binding=1) buffer PosOut {
  vec4 PositionOut[];
};
layout(std430, binding=2) buffer VelIn {
  vec4 VelocityIn[];
};
layout(std430, binding=3) buffer VelOut {
  vec4 VelocityOut[];
};

layout(std430,binding=5) buffer springupin
{
  uint springUp[];
};
layout(std430,binding=6) buffer springdownin
{
  uint springDown[];
};

vec3 applySprings(uvec3 nParticles ,uint idx , vec3 p , vec3 v )
{
  vec3 force = vec3(0),r ;

  // Particle above
  if( gl_GlobalInvocationID.y < nParticles.y - 1 ) {
    r = PositionIn[idx + nParticles.x].xyz - p;
    force += normalize(r) * SpringK * (length(r) - RestLengthVert);
  } 
  // Below
  if( gl_GlobalInvocationID.y > 0   ) {
    r = PositionIn[idx - nParticles.x].xyz - p;
    force += normalize(r) * SpringK * (length(r) - RestLengthVert);
  } 
  // Left
  if( gl_GlobalInvocationID.x > 0 ) {
    r = PositionIn[idx-1].xyz - p;
    force += normalize(r) * SpringK * (length(r) - RestLengthHoriz);
  } 
  // Right
  if( gl_GlobalInvocationID.x < nParticles.x - 1 ) {
    r = PositionIn[idx + 1].xyz - p;
    force += normalize(r) * SpringK * (length(r) - RestLengthHoriz);
  }

  // Diagonals
  // Upper-left
  if( gl_GlobalInvocationID.x > 0 && gl_GlobalInvocationID.y < nParticles.y - 1 ) {
    r = PositionIn[idx + nParticles.x - 1].xyz - p;
    force += normalize(r) * SpringK * (length(r) - RestLengthDiag);
  }
  // Upper-right
  if( gl_GlobalInvocationID.x < nParticles.x - 1 && gl_GlobalInvocationID.y < nParticles.y - 1 ) {
    r = PositionIn[idx + nParticles.x + 1].xyz - p;
    force += normalize(r) * SpringK * (length(r) - RestLengthDiag);
  }
  // lower -left
  if( gl_GlobalInvocationID.x > 0 && gl_GlobalInvocationID.y > 0  ) {
    r = PositionIn[idx - nParticles.x - 1].xyz - p;
    force += normalize(r) * SpringK * (length(r) - RestLengthDiag);
  }
  // lower-right
  if( gl_GlobalInvocationID.x < nParticles.x - 1 && gl_GlobalInvocationID.y > 0) {
    r = PositionIn[idx - nParticles.x + 1].xyz - p;
    force += normalize(r) * SpringK * (length(r) - RestLengthDiag);
  }
return force;
}

void main() {
  uvec3 nParticles = gl_NumWorkGroups * gl_WorkGroupSize;
  uint idx = gl_GlobalInvocationID.y * nParticles.x + gl_GlobalInvocationID.x;

  vec3 p = vec3(PositionIn[idx]);
  vec3 v = vec3(VelocityIn[idx]) , force = vec3(0);


  force = applySprings(nParticles,idx,p,v);

  force += Gravity * ParticleMass;
  force += -DampingConst * v;


/// sphere coll
  vec3 dic =p-SpherePos;
  float l =length(dic);
  if(l<SphereR)
  {
 force+= normalize(dic)*(SphereR-l)*250;
  }


if (idx ==selectedVert)
{

  force+=mouseForce;
}

  // Euler integrator
  vec3 a = force * ParticleInvMass;
  PositionOut[idx] = vec4(
      p + v * DeltaT + 0.5 * a * DeltaT * DeltaT, 1.0);
  VelocityOut[idx] = vec4( v + a * DeltaT, 0.0);



/// pin points
  if( gl_GlobalInvocationID.y == nParticles.y - 1 && 
      (gl_GlobalInvocationID.x == 0 || 
       gl_GlobalInvocationID.x == nParticles.x / 6 ||
        gl_GlobalInvocationID.x == nParticles.x * 2 / 6 ||
       gl_GlobalInvocationID.x == nParticles.x * 3 / 6 ||
       gl_GlobalInvocationID.x == nParticles.x * 4 / 6 ||
        gl_GlobalInvocationID.x == nParticles.x * 5 / 6 ||
       gl_GlobalInvocationID.x == nParticles.x - 1)) {
    PositionOut[idx] = vec4(p,1.0);
    VelocityOut[idx] = vec4(0,0,0,0);
  }
}

